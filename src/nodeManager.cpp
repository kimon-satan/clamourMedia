//

//  nodeManager.cpp
//  clamourMedia
//
//  Created by Simon Katan on 06/09/2013.
//
//

#include "nodeManager.h"



nodeManager::nodeManager() {


    //populate nodeArray
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {

            string t_index = ofToString(char(65 + j)) + "_" + ofToString(i + 1);
            ofPtr<clamourNode> t_mn = ofPtr<clamourNode>( new clamourNode(i + 1, ofToString(char(65 + j))));
            t_mn->setDrawType("DEBUG");
            mNodes[t_index] = t_mn;


        }
    }

}

nodeManager::nodeManager(vector<string> indexes) {


    //populate nodeArray
    for(int i = 0; i < indexes.size(); i++) {

        string r = indexes[i].substr(0,1);
        int s = ofToInt(indexes[i].substr(2));
        ofPtr<clamourNode> t_mn = ofPtr<clamourNode>( new clamourNode(s,r));
        t_mn->setDrawType("DEBUG");
        mNodes[indexes[i]] = t_mn;


    }

    for(int i = 0; i < 16; i ++) {
        vector<ofPtr<clamourNode> >t;
        mCollisionMap.push_back(t);
    }




}


void nodeManager::setupRegions() {

    mCollisionMapBounds.clear();
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

    }

}

void nodeManager::beginShift(string t_index, float x, float y) {

    mNodes[t_index]->updateHistory(); //set the mean pos to the raw Pos
    //  mNodes[t_index]->setRawPos_rel(mNodes[t_index]->getMeanPos_rel()); // update the rawPos
    //  mNodes[t_index]->clearHistory();
    mNodes[t_index]->resetShift(x, y);

}


void nodeManager::updateNodes() {

    appReactions.clear();

    //implement scheduled commands
    vector<eventComm>::iterator e_it = mFutureEvents.begin();

    while(e_it != mFutureEvents.end()) {
        if(e_it->execAt == ofGetFrameNum()) {
            if(mNodes.find(e_it->ownerIndex) != mNodes.end()){
                implementReaction(e_it->r, mNodes[e_it->ownerIndex]);
            }
            e_it = mFutureEvents.erase(e_it);
        } else {
            ++e_it;
        }
    }


    for(int i = 0; i < mCollisionMap.size(); i++)mCollisionMap[i].clear();

    map<string, ofPtr<clamourNode> >::iterator it;

    it = mNodes.begin();


    while(it != mNodes.end()) {

        it->second->update();

        if(it->second->getCanSleep()) {
            it->second->setIsSleeping(!it->second->getIsFiring());
        }

        if(!it->second->getIsSleeping()) {
            it->second->updateHistory();
            it->second->updateDrawData();
            it->second->updateSoundData();
            it->second->reconcileSlaves();
            it->second->updateRotHistory();
            it->second->updatePath();
            it->second->updateEvents();
            if(it->second->getIsCollidable())addToCollisionRegions(it->second);
        }

        ++it;

    }



    it = mNodes.begin();

    //now check for collisions
    while(it != mNodes.end()) {
        if(!it->second->getIsSleeping()) {
            if(it->second->getIsCollidable())checkForCollisions(it->second);
        }
        ++it;
    }
}

void nodeManager::addToCollisionRegions(ofPtr<clamourNode> n) {

    ofRectangle r = n->getBounds();

    float nFar_x = r.x + r.width;
    float nFar_y = r.y + r.height;

    vector <ofRectangle> :: iterator it =  mCollisionMapBounds.begin();
    int t_count = 0;

    while(it != mCollisionMapBounds.end()) {

        float tFar_x = it->x + it->width;
        float tFar_y = it->y + it->height;

        if(
            (
                it->inside(ofPoint(r.x,r.y)) ||
                it->inside(ofPoint(r.x,nFar_y)) ||
                it->inside(ofPoint(nFar_x,r.y)) ||
                it->inside(ofPoint(nFar_x,nFar_y))
            )
            ||
            (
                ((it->x >= r.x && it->x <= nFar_x) ||
                 ( tFar_x <= nFar_x &&  tFar_x >= r.x)
                )
                &&
                ((it->y >= r.y && it->y <= nFar_y) ||
                 ( tFar_y <= nFar_y &&  tFar_y >= r.y)
                )
            )
        ) {

            mCollisionMap[t_count].push_back(n);

        }
        t_count ++;
        ++it;
    }

}

void nodeManager::checkForCollisions(ofPtr<clamourNode> n) {

    bool collisionFound = false;

    for(int  i = 0; i < mCollisionMapBounds.size(); i ++) {

        if(mCollisionMapBounds[i].inside(n->getMeanPos_abs())) {

            vector <ofPtr<clamourNode> >::iterator target;

            target = mCollisionMap[i].begin();

            while(target != mCollisionMap[i].end()) {

                if(*target != n) {

                    if((*target)->getBounds().inside(n->getMeanPos_abs())){
                        bool isCollision = clamourUtils::pointInPath((*target)->getOuterEdge(), n->getMeanPos_abs());
                        if(isCollision){
                            if(!n->getIsColliding() && !(*target)->getIsColliding())implementReactions(n , *target);
                            n->setIsColliding(true);
                            (*target)->setIsColliding(true);
                            collisionFound = true;
                        }
                    }

                }

                ++target;
            }


        }

    }

    n->setIsColliding(collisionFound);

}

void nodeManager::implementReactions(ofPtr<clamourNode> n, ofPtr<clamourNode> tgt){

    vector<reaction> r = n->getReactions();
    vector<reaction>::iterator it = r.begin();

    while(it != r.end()){

        if(it->floatParams.find("DELAY_SECS") != it->floatParams.end()) {

            eventComm e;
            int delFrames = it->floatParams["DELAY_SECS"] * ofGetFrameRate();
            e.execAt = ofGetFrameNum() + delFrames;
            if(it->intParams.find("ENV_INDEX") != it->intParams.end())e.eventIndex = it->intParams["ENV_INDEX"];
            e.ownerIndex = n->getName();
            e.r = *it;
            mFutureEvents.push_back(e);


        }else{
            //implement immediately
            implementReaction(*it, tgt);
        }

        ++it;
    }



}

void nodeManager::implementReaction(reaction & r, ofPtr<clamourNode> t){

    if(r.rType == "sleep"){

        appReactions[t->getName()] = "resetControl";
        killNode(t->getName());

   }else if(r.rType == "transform"){

        clamourNode temp = presetStore::nodePresets[r.stringParams["PRESET"]]; //load the node into the reaction for easier variation
        setNode(t, temp);

    } else if(r.rType == "scaleNode") {

        parameter p = t->getDrawData().getParameter("size");

        p.abs_val *= r.floatParams["SCALE"];
        p.abs_val = min(1.0f, p.abs_val);
        t->setDrawParameter(p);
        ofPath pt = t->getEdgeTemplate();
        pt.scale(r.floatParams["SCALE"], r.floatParams["SCALE"]);
        t->setEdgeTemplate(pt);
        t->updatePath();

    } else if(r.rType == "scaleShift") {

        float shift = t->getShiftAmount() * r.floatParams["SCALE"];
        t->setShiftAmount(shift);

    } else if(r.rType == "scaleAttack") {

        float att = t->getAttSecs() * r.floatParams["SCALE"];
        t->setAttSecs(att);

    } else if(r.rType == "event") {

        t->triggerEvent(r.intParams["ENV_INDEX"]);


    } else if(r.rType == "eventOff") {

        t->endEvent(r.intParams["ENV_INDEX"]); // not sure this one is necessary

    }

}




void nodeManager::distributeNodes(vector<string> clients, string pattern, map<string, float> params, bool dimp, bool posp) {

    float w_prop = (float)screenData::width/screenData::height;

    if(pattern == "RANDOM_CIRCLE") {

        ofVec2f c(params["X"], params["Y"]);
        if(posp)c *= ofVec2f(w_prop,1.0);
        float r = params["RADIUS"];

        for(int i = 0; i < clients.size(); i++) {

            ofVec2f p(0,ofRandom(0,r));
            p = p.getRotated(ofRandom(-180,180));
            if(dimp)p *= ofVec2f(w_prop,1.0);
            mNodes[clients[i]]->setRawPos_abs(p+c);
            mNodes[clients[i]]->setAnimOverride(20);

        }

    } else if(pattern == "CIRCUM") {

        ofVec2f c(params["X"], params["Y"]);
        if(posp)c *= ofVec2f(w_prop,1.0);
        float r = params["RADIUS"];

        float interval = (float)360.0/clients.size();

        for(int i = 0; i < clients.size(); i++) {

            ofVec2f p(0,r);
            p = p.getRotated(-180 + i * interval);
            if(dimp)p *= ofVec2f(w_prop,1.0);
            mNodes[clients[i]]->setRawPos_abs(p+c);
            mNodes[clients[i]]->setAnimOverride(20);
        }

    } else if(pattern == "XY") {

        ofVec2f c(params["X"], params["Y"]);
        if(posp)c *= ofVec2f(w_prop,1.0);


        for(int i = 0; i < clients.size(); i++) {

            mNodes[clients[i]]->setRawPos_abs(c);
            mNodes[clients[i]]->setAnimOverride(20);
        }

    }
}


void nodeManager::switchOffAllNodes() {

    map<string, ofPtr<clamourNode> >::iterator it;

    it = mNodes.begin();

    while(it != mNodes.end()) {
        switchOffNode(it->second->getName());
        ++it;
    }

}


void nodeManager::switchOffNodes(vector<string> v) {

    //change to flags
    for(int i = 0; i < v.size(); i ++) {
        switchOffNode(v[i]);
    }


}

void nodeManager::switchOffNode(string t_index) {

    if(mNodes[t_index]->getIsFired()) {
        mNodes[t_index]->setIsFired(false);
        if(mNodes[t_index]->getCanSleep())mNodes[t_index]->setIsSleeping(true);
        if(mNodes[t_index]->getEnvType() == CLAMOUR_ASR)mNodes[t_index]->setChanged(CLAMOUR_ON_OFF);
        mNodes[t_index]->clearHistory();
    } else {
        if(mNodes[t_index]->getIsSleeping())mNodes[t_index]->setIsSleeping(true);
    }



}

void nodeManager::switchOnNode(string t_index) {
    if(!mNodes[t_index]->getIsFired()) {
        mNodes[t_index]->setIsFired(true);
        mNodes[t_index]->react();
        mNodes[t_index]->setChanged(CLAMOUR_ON_OFF);
    }
}

void nodeManager::switchOnNode(string t_index, float x, float y) {

    mNodes[t_index]->setRawPos_rel(ofVec2f(x, y));
    switchOnNode(t_index);

}

void nodeManager::killAllNodes(){

    map<string, ofPtr<clamourNode> >::iterator it;

    it = mNodes.begin();

    while(it != mNodes.end()) {
        killNode(it->first);
        ++it;
    }

}

void nodeManager::killNodes(vector<string> indexes){

    for(int i = 0; i < indexes.size(); i ++) {
        killNode(indexes[i]);
    }

}

void nodeManager::killNode(string t_index){

    setNode(mNodes[t_index], presetStore::nodePresets["defaultNode"]);

}

void nodeManager::wakeupNode(string t_index) {

    mNodes[t_index]->setIsSleeping(mNodes[t_index]->getCanSleep());

}


void nodeManager::updateNodePosition(string t_index, float x, float y) {
    mNodes[t_index]->setRawPos_rel(ofVec2f(x,y));
    if(mNodes[t_index]->getChanged() == CLAMOUR_NONE)mNodes[t_index]->setChanged(CLAMOUR_POSITION); //don't let position override on/off messages
}

void nodeManager::shiftNodePosition(string t_index, float x, float y) {

    if(!mNodes[t_index]->getIsDragOn())return;

    ofVec2f s(x,y);
    if(mNodes[t_index]->getAnimOverride() > 0 || mNodes[t_index]->getIsNewShift()) {
        cout << "reset \n" << endl;
        mNodes[t_index]->resetShift(x,y);
        return;
    }
    ofVec2f p = mNodes[t_index]->getShiftStart() + s * mNodes[t_index]->getShiftAmount();
    mNodes[t_index]->setRawPos_rel(p);
    if(mNodes[t_index]->getChanged() == CLAMOUR_NONE)mNodes[t_index]->setChanged(CLAMOUR_POSITION);

}



map<string, ofPtr<clamourNode> > nodeManager::getNodes() {
    return mNodes;
}

ofVec2f nodeManager::getNodePosition(string index, bool isRel) {

    return (isRel) ? mNodes[index]->getMeanPos_rel() : mNodes[index]->getMeanPos_abs();

}

ofPtr<clamourNode> nodeManager::getNode(string index) {

    return mNodes[index];
}

void nodeManager::setNodeAttSecs(vector<string> indexes, float att) {

    for(int i = 0; i < indexes.size(); i ++)mNodes[indexes[i]]->setAttSecs(att);

}

void nodeManager::setNodeDecSecs(vector<string> indexes, float dec) {

    for(int i = 0; i < indexes.size(); i ++)mNodes[indexes[i]]->setDecSecs(dec);

}


void nodeManager::setNodeDraw(vector<string> indexes, baseData &bd) {

    for(int i = 0; i < indexes.size(); i ++) {
        mNodes[indexes[i]]->setDrawData(bd);
    }

}


void nodeManager::setNodeSound(vector<string> indexes, baseData &bd) {

    for(int i = 0; i < indexes.size(); i ++) {
        mNodes[indexes[i]]->setSoundData(bd);
        if(mNodes[indexes[i]]->getIsFiring())mNodes[indexes[i]]->setChanged(CLAMOUR_SOUND);

    }

}


void nodeManager::setNodeDrawParam(vector<string> indexes, parameter &p) {

    for(int i = 0; i < indexes.size(); i ++) {

        p.init(mNodes[indexes[i]]->getMeanPos_rel()); //if mapped randomly only reset that parameter
        mNodes[indexes[i]]->setDrawParameter(p);

    }

}


void nodeManager::setNodeSoundParam(vector<string> indexes, parameter &p) {

    //NB this method assumes all nodes are of the same soundType

    for(int i = 0; i < indexes.size(); i ++) {

        p.init(mNodes[indexes[i]]->getMeanPos_rel());//if mapped randomly only reset that parameter // why not in the class ?
        mNodes[indexes[i]]->setSoundParameter(p);

    }

}

void nodeManager::setNodes(vector<string> indexes, clamourNode &n) {

    for(int i = 0; i < indexes.size(); i ++) {

        nodeManager::setNode(mNodes[indexes[i]], n);

    }

}

void nodeManager::setNode(ofPtr<clamourNode> target, clamourNode &temp) {

    target->endEvents();
    target->endSound();
    target->setSoundData(temp.getSoundData());
    target->setChanged(CLAMOUR_SOUND);

    target->setDrawData(temp.getDrawData());
    target->setEnvType(temp.getEnvType());
    target->setAttSecs(temp.getAttSecs());
    target->setDecSecs(temp.getDecSecs());
    target->setCanSleep(temp.getCanSleep());
    target->setIsRotate(temp.getIsRotate());
    target->setShiftAmount(temp.getShiftAmount());
    target->setEvents(temp.getEvents());
    target->setReactions(temp.getReactions());
    target->setSounds(temp.getSounds());
    target->setIsCollidable(temp.getIsCollidable());
    target->init();

    //now create an edgeTemplate

    if(target->getDrawData().getName() != "none") {

        ofPath p;
        pathFactory::createPath(p, target->getDrawData().getShapeType(),
                                1.0,1.0,
                                target->getDrawData().getParameter("size").abs_val);

        target->setEdgeTemplate(p);
    }


}


map<string, string> nodeManager::getAppReactions(){
    return appReactions;
}




