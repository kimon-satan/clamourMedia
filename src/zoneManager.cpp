#include "zoneManager.h"

zoneManager::zoneManager()
{
    //ctor
}

void zoneManager::update(map<string, ofPtr<clamourNode> > tNodes)
{

    map<string, ofPtr<zone> >::iterator z_it;
    map<string, ofPtr<clamourNode> >::iterator n_it;

    //find node intersections
    n_it = tNodes.begin();

    while(n_it != tNodes.end())
    {

        if(n_it->second->getIsSleeping())
        {

            if(n_it->second->getZonePair())
            {
                if(!n_it->second->getZonePair()->getIsClosedIn()){
                    n_it->second->getZonePair()->removeNode(n_it->second);
                    n_it->second->resetZonePair();
                }
            }

            ++n_it;
            continue;

        }

        for(z_it = mZones.begin(); z_it != mZones.end(); ++z_it)
        {

            if(z_it->second->getIsClosedIn()){

                if(n_it->second->getZonePair() == z_it->second){
                    //only for already captured nodes
                    containNode(n_it->second, z_it->second);
                    continue; // mustn't allow other reactions

                }

            }

            if(checkInZone(n_it->second, z_it->second)){


                if(n_it->second->getZonePair() == z_it->second){
                    break; //already in the zone
                }

                //new nodes inside the zone

                if(z_it->second->getIsClosedOut()){
                    repellNode(n_it->second, z_it->second);
                }else{


                    if(n_it->second->getZonePair())
                    {
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

            }
            else if(n_it->second->getZonePair() == z_it->second){
                z_it->second->removeNode(n_it->second);
                n_it->second->resetZonePair();
            }

        }

        ++n_it;
    }

    //call update method here which resets closed zones and increments reactions etc

    for(z_it = mZones.begin(); z_it != mZones.end(); ++z_it)
    {
        z_it->second->update();
        if(getOffTrig(z_it->second)){
            offReact(z_it->second);
        }

    }


}

bool zoneManager::checkInZone(ofPtr<clamourNode> n, ofPtr<zone> z){

    return clamourUtils::pointInPath(z->getOuterEdge(), n->getMeanPos_abs());
}

void zoneManager::repellNode(ofPtr<clamourNode> n, ofPtr<zone> z){

    //finding centroid is not solved here so this method only works if zones drawn from the center

    ofPoint p = clamourUtils::getInsideIntersect(z->getOuterEdge(), z->getPos_abs(), n->getMeanPos_abs());
    n->setRawPos_abs(p);
    n->modifyHistory();

}

void zoneManager::containNode(ofPtr<clamourNode> n, ofPtr<zone> z){

    ofVec2f v(n->getMeanPos_abs() - z->getPos_abs());

    ofPoint p = clamourUtils::getInsideIntersect(z->getOuterEdge(), z->getPos_abs(), n->getMeanPos_abs());
    ofVec2f d = (p - z->getPos_abs());
    p -= d * 0.05; //move it slightly inside

    if(d.length() * 0.95 < v.length()){ //only if necessary
        n->setRawPos_abs(p);
        n->modifyHistory();
    }

}

bool zoneManager::getIsRuleMet(ofPtr<zone> z, zoneRule r){


    if(r.ruleType == "MIN_OCCUPANTS"){

        return(z->getCaptureNodes().size() >= r.gtOccupants);

    }else if(r.ruleType == "MAX_OCCUPANTS"){

        if(z->getCaptureNodes().size() > 0 &&
           z->getCaptureNodes().size() <= r.ltOccupants)return true;

    }else if(r.ruleType == "RANGE_OCCUPANTS"){

        if(z->getCaptureNodes().size() >= r.gtOccupants  &&
           z->getCaptureNodes().size() <= r.ltOccupants)return true;

    }else{

      return false;

    }

    return false;

}

bool zoneManager::getOnTrig(ofPtr<zone> z){

    if(z->getIsFired())return false; //this could be variable

    zoneRule on_zr = z->getOnRule();
    zoneRule off_zr = z->getOffRule();

    if(!off_zr.isEnabled){
        return getIsRuleMet(z,on_zr);
    }else{
        return getIsRuleMet(z, off_zr)? false : getIsRuleMet(z,on_zr); //off overrides on
    }

}

bool zoneManager::getOffTrig(ofPtr<zone> z){


    if(!z->getIsFired())return false;

    zoneRule on_zr = z->getOnRule();
    zoneRule off_zr = z->getOffRule();

    if(!off_zr.isEnabled){
        return !getIsRuleMet(z,on_zr);// will fire when on rule is no longer met
    }else{
        return getIsRuleMet(z, off_zr); // off overrides on
    }

}


void zoneManager::onReact(ofPtr<zone> z){

    z->setIsFired(true);
    z->react();
    z->setChanged(CLAMOUR_ON_OFF);
    implementReactions(z, true);

}

void zoneManager::offReact(ofPtr<zone> z){

    z->setIsFired(false);
    if(z->getEnvType() == CLAMOUR_ASR)z->setChanged(CLAMOUR_ON_OFF);
    implementReactions(z, false);

}

void zoneManager::implementReactions(ofPtr<zone> z, bool isOn){


    vector<reaction> r = z->getReactions(); //TODO copy the reactions out and replace them at the end of this method
                                                     // ptrs don't work here
    vector<reaction>::iterator it = r.begin();

    while(it != r.end()){

        if((it->trig == "ON" && !isOn) ||
            (it->trig == "OFF" && isOn)){
                ++it;
                continue;
            }

        bool isReverse = (it->trig == "ON_OFF" && !isOn);

        if(it->rType == "closeInZone"){
             z->setIsClosedIn(!isReverse);
        }else if(it->rType == "openInZone"){
             z->setIsClosedIn(isReverse);
        }else if(it->rType == "closeOutZone"){
            z->setIsClosedOut(!isReverse);
        }else if(it->rType == "openOutZone"){
            z->setIsClosedOut(isReverse);
        }
        ++it;
    }

    z->setReactions(r);

};

void zoneManager::createZone(string name)
{

    //there will be alot more detail here at some point
    //perhaps pass the zone from the main app
    ofPtr<zone> z = ofPtr<zone>(new zone());
    mZones[name] = z;



}



void zoneManager::createZone(zone z)
{

    ofPtr<zone> zp = ofPtr<zone>(new zone(z));
    zp->recalcAbsDims();
    mZones[zp->getName()] = zp;

}

void zoneManager::hideZone(string name)
{

}

void zoneManager::showZone(string name)
{

}

void zoneManager::destroyZone(string name)
{


    if(mZones[name]->getCaptureNodes().size() > 0)
    {

        map<string, ofPtr<clamourNode> > :: iterator it = mZones[name]->getCaptureNodes().begin();

        while(it != mZones[name]->getCaptureNodes().end())
        {

            it->second->resetZonePair();

            ++it;
        }

    }

    mZones.erase(name);

}

void zoneManager::destroyAllZones()
{

    map<string, ofPtr<zone> > :: iterator zit = mZones.begin();

    while(zit != mZones.end())
    {

        if(zit->second->getCaptureNodes().size() == 0)
        {
            ++zit;
            continue;
        }

        map<string, ofPtr<clamourNode> > :: iterator nit = zit->second->getCaptureNodes().begin();


        while(nit != zit->second->getCaptureNodes().end())
        {

            nit->second->resetZonePair();

            ++nit;
        }

        ++ zit;
    }

    mZones.clear();

}

void zoneManager::setZoneDraw(vector<string> indexes, baseData &bd)
{

    for(int i = 0; i < indexes.size(); i ++)mZones[indexes[i]]->setDrawData(bd);

}


void zoneManager::setZoneSound(vector<string> indexes, baseData &bd){

    for(int i = 0; i < indexes.size(); i ++)
    {
        mZones[indexes[i]]->setSoundData(bd);
        if(mZones[indexes[i]]->getIsFiring())mZones[indexes[i]]->setChanged(CLAMOUR_SOUND);

    }

}


void zoneManager::setZoneDrawParam(vector<string> indexes, parameter &p)
{

    for(int i = 0; i < indexes.size(); i ++)
    {

        //FIX_ME either give the zone a position or parameter needs a non position initializer

        p.init(ofVec2f(0.5,0.5)); //if mapped randomly only reset that parameter
        mZones[indexes[i]]->setDrawParameter(p);

    }

}


void zoneManager::setZoneSoundParam(vector<string> indexes, parameter &p)
{

    //assumes all zones of the same draw type

    parameter t = mZones[indexes[0]]->getSoundData().getParameter(p.name); //a copy of the original
    p.index = t.index; //copy these over
    p.warp = t.warp;

    for(int i = 0; i < indexes.size(); i ++)
    {

        p.init(ofVec2f(0.5,0.5));//if mapped randomly only reset that parameter
        mZones[indexes[i]]->setSoundParameter(p);

    }

}

map<string, ofPtr<zone> > zoneManager::getZones()
{
    return mZones;
}

zoneManager::~zoneManager()
{
    //dtor
}
