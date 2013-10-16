#include "zoneManager.h"

zoneManager::zoneManager() {
    //ctor
}

void zoneManager::setupRegions() {

   /* mCollisionMapBounds.clear();
    float p = (float)screenData::width/screenData::height;

    for(int i = 0; i < 4; i++) {

        for(int j = 0; j < 4; j ++) {

            ofRectangle r;
            r.x = 0.25 * p * i;
            r.y = 0.25 * j;
            r.width = 0.25 * p;
            r.height = 0.25;
            mCollisionMapBounds.push_back(r);
        }

    }*/

}


void zoneManager::update(map<string, ofPtr<clamourNode> > tNodes) {

    appReactions.clear();

    //implement scheduled commands
    vector<eventComm>::iterator e_it = mFutureEvents.begin();

    while(e_it != mFutureEvents.end()) {
        if(e_it->execAt == ofGetFrameNum()) {
            if(mZones.find(e_it->ownerIndex) != mZones.end()){
                implementReaction(e_it->r, mZones[e_it->ownerIndex]);
            }
            e_it = mFutureEvents.erase(e_it);
        } else {
            ++e_it;
        }
    }

    map<string, ofPtr<zone> >::iterator z_it;
    map<string, ofPtr<clamourNode> >::iterator n_it;

    //call update method here which resets closed zones and increments reactions etc

    for(z_it = mZones.begin(); z_it != mZones.end(); ++z_it) {
        z_it->second->update();
        z_it->second->updateEvents();
        z_it->second->updateDrawData();
        if(getOffTrig(z_it->second)) {
            offReact(z_it->second);
        }

    }



    //find node intersections
    n_it = tNodes.begin();

    while(n_it != tNodes.end()) {

        if(n_it->second->getIsSleeping()) {

            if(n_it->second->getZonePair()) {
                if(!n_it->second->getZonePair()->getIsClosedIn()) {
                    n_it->second->getZonePair()->removeNode(n_it->second);
                    n_it->second->resetZonePair();
                }
            }

            ++n_it;
            continue;

        }


        for(z_it = mZones.begin(); z_it != mZones.end(); ++z_it) {

            if(z_it->second->getIsClosedIn()) {

                if(n_it->second->getZonePair() == z_it->second) {
                    //only for already captured nodes
                    containNode(n_it->second, z_it->second);
                    continue; // mustn't allow other reactions

                }

            }

            if(checkInZone(n_it->second, z_it->second)) {


                if(n_it->second->getZonePair() == z_it->second) {
                    break; //already in the zone
                }

                //new nodes inside the zone

                if(z_it->second->getIsClosedOut()) {
//                    cout << "repell \n";
                    repellNode(n_it->second, z_it->second);
                } else {


                    if(n_it->second->getZonePair()) {
                        //incase the node has jumped straight from one zone to the next
                        n_it->second->getZonePair()->removeNode(n_it->second);
                        n_it->second->resetZonePair();
                    }

                    n_it->second->setZonePair(z_it->second);
                    z_it->second->addNode(n_it->second);

                    //now update zone reactions
                    if(getOnTrig(z_it->second))onReact(z_it->second);

                    break; //no need to check any more zones for this node
                }

            } else if(n_it->second->getZonePair() == z_it->second) {
                z_it->second->removeNode(n_it->second);
                n_it->second->resetZonePair();
            }

        }

        ++n_it;
    }




}

bool zoneManager::checkInZone(ofPtr<clamourNode> n, ofPtr<zone> z) {

//    cout << "check in zone \n";
    //  return clamourUtils::pointInPath(z->getOuterEdge(), n->getMeanPos_abs());
    ofPoint intersect;
    bool isIntersect = clamourUtils::pathInPath( n->getOuterEdge(), z->getOuterEdge(), intersect);
    if(isIntersect)n->setIntersect(intersect);
    return isIntersect;

}

void zoneManager::repellNode(ofPtr<clamourNode> n, ofPtr<zone> z) {

//    cout << "repell \n";
    //finding centroid is not solved here so this method only works if zones drawn from the center
    ofVec2f v = n->getMeanPos_abs() - n->getIntersect();
    ofPoint p = clamourUtils::getInsideIntersect(z->getOuterEdge(), z->getPos_abs(), n->getIntersect());
    p = p + v;
    n->setRawPos_abs(p);
    n->modifyHistory();

}

void zoneManager::containNode(ofPtr<clamourNode> n, ofPtr<zone> z) {

    ofPoint intersect;
    bool isOutside = clamourUtils::pathOutPath( n->getOuterEdge(), z->getOuterEdge(), intersect);

    if(isOutside) {
        ofVec2f v = n->getMeanPos_abs() - intersect;
        ofVec2f vi(intersect - z->getPos_abs());

        ofPoint p = clamourUtils::getInsideIntersect(z->getOuterEdge(), z->getPos_abs(), intersect);
        ofVec2f d = (p - z->getPos_abs());
        p -= d * 0.05; //move it slightly inside
        p = p + v;

        if(d.length() * 0.95 < vi.length()) { //only if necessary
            n->setRawPos_abs(p);
            n->modifyHistory();
        }

    }

}

bool zoneManager::getIsRuleMet(ofPtr<zone> z, zoneRule r) {


    if(r.ruleType == "MIN_OCCUPANTS") {

        return(z->getCaptureNodes().size() >= r.gtOccupants);

    } else if(r.ruleType == "MAX_OCCUPANTS") {

        if(z->getCaptureNodes().size() > 0 &&
                z->getCaptureNodes().size() <= r.ltOccupants)return true;

    } else if(r.ruleType == "RANGE_OCCUPANTS") {

        if(z->getCaptureNodes().size() >= r.gtOccupants  &&
                z->getCaptureNodes().size() <= r.ltOccupants)return true;

    } else {

        return false;

    }

    return false;

}

bool zoneManager::getOnTrig(ofPtr<zone> z) {

    if(z->getIsFired())return false; //this could be variable

    zoneRule on_zr = z->getOnRule();
    zoneRule off_zr = z->getOffRule();

    if(!on_zr.isEnabled) {
        return true;
    } else if(!off_zr.isEnabled) {
        return getIsRuleMet(z,on_zr);
    } else {
        return getIsRuleMet(z, off_zr)? false : getIsRuleMet(z,on_zr); //off overrides on
    }

}

bool zoneManager::getOffTrig(ofPtr<zone> z) {

    if(!z->getIsFired())return false;

    zoneRule on_zr = z->getOnRule();
    zoneRule off_zr = z->getOffRule();

    if(!on_zr.isEnabled) {
        return true;
    } else if(!off_zr.isEnabled) {
        return !getIsRuleMet(z,on_zr);// will fire when on rule is no longer met
    } else {
        return getIsRuleMet(z, off_zr); // off overrides on
    }

}


void zoneManager::onReact(ofPtr<zone> z) {

    z->setIsFired(true);
    z->react();
    z->setChanged(CLAMOUR_ON_OFF); //still needed ?
    scMessenger::startSynth(z->getName(), *z, z->getSoundData());
    implementReactions(z, true);

}

void zoneManager::offReact(ofPtr<zone> z) {

    z->setIsFired(false);

    if(z->getEnvType() == CLAMOUR_ASR){
        scMessenger::stopSynth(z->getName(), z->getSoundData());
        z->setChanged(CLAMOUR_ON_OFF); // is this still needed
    }
    implementReactions(z, false);

}

void zoneManager::implementReactions(ofPtr<zone> z, bool isOn) {


    vector<reaction> r = z->getReactions(); //TODO copy the reactions out and replace them at the end of this method
    // ptrs don't work here
    vector<reaction>::iterator it = r.begin();

    while(it != r.end()) {

        if((it->trig == "ON" && !isOn) ||
                (it->trig == "OFF" && isOn)) {
            ++it;
            continue;
        }

        bool isReverse = (it->trig == "ON_OFF" && !isOn);

        vector<ofPtr<zone> > zt;

        if(it->zTargets.size() == 0){
            zt.push_back(z);
        }else{
            for(int i = 0; i < it->zTargets.size(); i++) {
                if(mZones.find(it->zTargets[i]) != mZones.end())zt.push_back(mZones[it->zTargets[i]]);
            }
        }

        for(int i = 0; i < zt.size(); i++) {

            if(it->floatParams.find("DELAY_SECS") != it->floatParams.end()) {

                //ON_OFF events not available for these
                //very messy definitely needs reworking

                eventComm e;
                int delFrames = it->floatParams["DELAY_SECS"] * ofGetFrameRate();
                e.execAt = ofGetFrameNum() + delFrames;
                if(it->intParams.find("ENV_INDEX") != it->intParams.end())e.eventIndex = it->intParams["ENV_INDEX"];
                e.ownerIndex = zt[i]->getName();
                e.r = *it;
                mFutureEvents.push_back(e);


            }else{
                //implement immediately
                implementReaction(*it, zt[i], isReverse);
            }

        }
         ++it;
    }

    //z->setReactions(r); NOT CURRENTLY STORING DATA

};

void zoneManager::implementReaction(reaction &r, ofPtr<zone> z, bool isReverse) {

    map<string, ofPtr<clamourNode> > cap = z->getCaptureNodes();

    if(r.rType == "closeInZone") {
        z->setIsClosedIn(!isReverse);
    } else if(r.rType == "openInZone") {
        z->setIsClosedIn(isReverse);
    } else if(r.rType == "closeOutZone") {
        z->setIsClosedOut(!isReverse);
    } else if(r.rType == "openOutZone") {
        z->setIsClosedOut(isReverse);
    } else if(r.rType == "incrementStage") {
        appReactions.push_back("incrementStage");
    } else if(r.rType == "repeatStage") {
        appReactions.push_back("repeatStage");
    } else if(r.rType == "decrementStage") {
        appReactions.push_back("decrementStage");
    } else if(r.rType == "muteSynths") {
        appReactions.push_back("muteSynths");
    } else if(r.rType == "transformNode") {
        //potentially could need on/off for data storage

        map<string, ofPtr<clamourNode> >::iterator c_it = cap.begin();
        clamourNode temp;

        if(!r.tempNode){
            temp = presetStore::nodePresets[r.stringParams["PRESET"]]; //load the node into the reaction for easier variation
        }else{
            temp = *r.tempNode;
        }

        while(c_it != cap.end()) {
            nodeManager::setNode(c_it->second, temp);
            ++ c_it;
        }
    } else if(r.rType == "scaleNode") {

        map<string, ofPtr<clamourNode> >::iterator c_it = cap.begin();

        while(c_it != cap.end()) {

            parameter p = c_it->second->getDrawData().getParameter("size");

            if((p.abs_val < 0.75 && r.floatParams["SCALE"] > 1 )|| ( p.abs_val > 0.01 && r.floatParams["SCALE"] < 1 )){
                p.abs_val *= r.floatParams["SCALE"];
                p.abs_val = min(1.0f, p.abs_val);
                c_it->second->setDrawParameter(p);
                ofPath pt = c_it->second->getEdgeTemplate();

                pt.scale(r.floatParams["SCALE"], r.floatParams["SCALE"]);
                c_it->second->setEdgeTemplate(pt);
                c_it->second->updatePath();
            }

            ++ c_it;
        }
    } else if(r.rType == "scaleShift") {

        map<string, ofPtr<clamourNode> >::iterator c_it = cap.begin();

        while(c_it != cap.end()) {
            float shift = c_it->second->getShiftAmount() * r.floatParams["SCALE"];
            c_it->second->setShiftAmount(shift);
            ++c_it;
        }

    } else if(r.rType == "scaleAttack") {

        map<string, ofPtr<clamourNode> >::iterator c_it = cap.begin();

        while(c_it != cap.end()) {

            float att = c_it->second->getAttSecs();
             if((att > 0.2 && r.floatParams["SCALE"] < 1 )|| ( att < 1 && r.floatParams["SCALE"] > 1 )){
                c_it->second->setAttSecs(att* r.floatParams["SCALE"]);
             }
            ++c_it;
        }

    } else if(r.rType == "event") {

        if(!isReverse){
            z->triggerEvent(r.intParams["ENV_INDEX"]);
        }else{
            z->endEvent(r.intParams["ENV_INDEX"]);
        }

    } else if(r.rType == "eventOff") {

        z->endEvent(r.intParams["ENV_INDEX"]);

    }
}

void zoneManager::implementComm(vector<string> zTargets, string comm){


    for(int i = 0; i < zTargets.size(); i++){

        if(comm == "closeOutZone"){
            mZones[zTargets[i]]->setIsClosedOut(true);
        }else if(comm == "openOutZone"){
            mZones[zTargets[i]]->setIsClosedOut(false);
        }

    }

}

void zoneManager::createZone(string name) {

  //  cout << "create \n";
    //there will be alot more detail here at some point
    //perhaps pass the zone from the main app
    ofPtr<zone> z = ofPtr<zone>(new zone());
    mZones[name] = z;

}



void zoneManager::createZone(zone z) {

  // cout << "create \n";
//    cout << z.getSoundFile() << endl;
    ofPtr<zone> zp = ofPtr<zone>(new zone(z));
    zp->recalcAbsDims();
    zp->init();
    mZones[zp->getName()] = zp;

}

void zoneManager::hideZone(string name) {

}

void zoneManager::showZone(string name) {

}

void zoneManager::destroyZone(string name) {

    mZones[name]->endEvents();
    mZones[name]->endSound();

    if(mZones[name]->getCaptureNodes().size() > 0) {

        map<string, ofPtr<clamourNode> > :: iterator it = mZones[name]->getCaptureNodes().begin();

        while(it != mZones[name]->getCaptureNodes().end()) {

            it->second->resetZonePair();

            ++it;
        }

    }

    mZones.erase(name);

}

void zoneManager::destroyAllZones() {

    map<string, ofPtr<zone> > :: iterator zit = mZones.begin();

    while(zit != mZones.end()) {

      destroyZone(zit->first);
      ++zit;
    }

    mZones.clear();

}

void zoneManager::setZoneDraw(vector<string> indexes, baseData &bd) {

    for(int i = 0; i < indexes.size(); i ++)mZones[indexes[i]]->setDrawData(bd);

}


void zoneManager::setZoneSound(vector<string> indexes, baseData &bd) {

    for(int i = 0; i < indexes.size(); i ++) {
        mZones[indexes[i]]->setSoundData(bd);
        if(mZones[indexes[i]]->getIsFiring())mZones[indexes[i]]->setChanged(CLAMOUR_SOUND);

    }
}

void zoneManager::setZoneDrawParam(vector<string> indexes, parameter &p) {

    for(int i = 0; i < indexes.size(); i ++) {

        //FIX_ME either give the zone a position or parameter needs a non position initializer

        p.init(ofVec2f(0.5,0.5)); //if mapped randomly only reset that parameter
        mZones[indexes[i]]->setDrawParameter(p);

    }

}

void zoneManager::setZoneReactions(vector<string> indexes,vector<reaction> r) {

    for(int i = 0; i < indexes.size(); i ++) {
        mZones[indexes[i]]->setReactions(r); //clears old reactions first
    }

}


void zoneManager::setZoneSoundParam(vector<string> indexes, parameter &p) {

    //assumes all zones of the same draw type

    parameter t = mZones[indexes[0]]->getSoundData().getParameter(p.name); //a copy of the original
    p.index = t.index; //copy these over
    p.warp = t.warp;

    for(int i = 0; i < indexes.size(); i ++) {

        p.init(ofVec2f(0.5,0.5));//if mapped randomly only reset that parameter
        mZones[indexes[i]]->setSoundParameter(p);

    }

}

map<string, ofPtr<zone> > zoneManager::getZones() {
    return mZones;
}

vector<string> zoneManager::getAppReactions() {

    return appReactions;

}

zoneManager::~zoneManager() {
    //dtor
}
