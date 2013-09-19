//
//  nodeManager.cpp
//  clamourMedia
//
//  Created by Simon Katan on 06/09/2013.
//
//

#include "nodeManager.h"

nodeManager::nodeManager(){

   // soundDictionary::setup(); //load in default synth parameters from XML

    //populate nodeArray
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){

            string t_index = ofToString(char(65 + j)) + "_" + ofToString(i + 1);
            ofPtr<clamourNode> t_mn = ofPtr<clamourNode>( new clamourNode(i + 1, ofToString(char(65 + j))));
            t_mn->setDrawType(CLAMOUR_DRAW_FLICKER);
            mNodes[t_index] = t_mn;

        }
    }


}

nodeManager::nodeManager(vector<string> indexes){

    //soundDictionary::setup(); //load in default synth parameters from XML

    //populate nodeArray
    for(int i = 0; i < indexes.size(); i++){

        string r = indexes[i].substr(0,1);
        int s = ofToInt(indexes[i].substr(2,1));
        ofPtr<clamourNode> t_mn = ofPtr<clamourNode>( new clamourNode(s,r));
        t_mn->setDrawType(CLAMOUR_DRAW_FLICKER);
        mNodes[indexes[i]] = t_mn;


    }


}


void nodeManager::beginShift(string t_index, float x, float y){

    mNodes[t_index]->resetShift(x, y);

}


void nodeManager::updateNodes(){

    //do not turn nodes off before here !

    for(int i = 0; i < onNodes.size(); i ++){

        string t_index = onNodes[i];
        mNodes[t_index]->updateHistory();
        mNodes[t_index]->updateDrawData();
        mNodes[t_index]->updateSoundData();

    }

    //purge old nodes
    for(int i = 0; i < offNodes.size(); i++){

        mNodes[offNodes[i]]->setIsOn(false);

        if(!mNodes[offNodes[i]]->getIsReturnToOn()){ // if the this is flagged then the node will be swtiched straight back on with a new synth
            vector<string>::iterator it = remove(onNodes.begin(), onNodes.end(), offNodes[i]);
            if(it != onNodes.end())onNodes.erase(it);
        }else{

            mNodes[offNodes[i]]->setIsReturnToOn(false);
        }

    }


    offNodes.clear(); //oscManager already used them so no need to retain

    //turn on any flagged nodes




    // any nodes turned off after here will be switched off in the subsequent frame






}

void nodeManager::distributeNodes(vector<string> clients, string pattern, map<string, float> params){

    if(pattern == "RANDOM_CIRCLE"){

        ofVec2f c(params["X"], params["Y"]);
        float r = params["RADIUS"];

        for(int i = 0; i < clients.size(); i++){

            ofVec2f p(0,ofRandom(0,r));
            p = p.getRotated(ofRandom(-180,180));
            mNodes[clients[i]]->setPosition(p+c);

        }

    }
}


void nodeManager::switchOffAllNodes(){

    //turn off all current nodes
    for(int i = 0; i < onNodes.size(); i ++){

        mNodes[onNodes[i]]->clearHistory();

    }

    offNodes = onNodes;


}


void nodeManager::switchOffNodes(vector<string> v){

    for(int i = 0; i < v.size(); i ++){

        mNodes[v[i]]->clearHistory();
        if(find(onNodes.begin(), onNodes.end(), v[i]) != onNodes.end())offNodes.push_back(v[i]);

    }


}

void nodeManager::switchOffNode(string t_index){

//duplicate methods

    mNodes[t_index]->clearHistory();
    if(find(onNodes.begin(), onNodes.end(), t_index) != onNodes.end())offNodes.push_back(t_index);

}

void nodeManager::switchOnNode(string t_index){

    if(find(onNodes.begin(), onNodes.end(), t_index) ==onNodes.end())onNodes.push_back(t_index);

}

void nodeManager::switchOnNode(string t_index, float x, float y){

    mNodes[t_index]->setPosition(ofVec2f(x, y));
    if(find(onNodes.begin(), onNodes.end(), t_index) ==onNodes.end())onNodes.push_back(t_index);

}





void nodeManager::updateNodePosition(string t_index, float x, float y){

    mNodes[t_index]->setPosition(ofVec2f(x,y));

}

void nodeManager::shiftNodePosition(string t_index, float x, float y){

    if(!mNodes[t_index]->getIsDragOn())return;
    ofVec2f s(x,y);
    ofVec2f p = mNodes[t_index]->getShiftStart() + s * mNodes[t_index]->getShiftAmount();

    mNodes[t_index]->setPosition(p);

}

void nodeManager::updateOnlineClients(vector<string> v){mOnlineClients = v;}
vector<string> nodeManager::getOnlineClients(){return mOnlineClients;}
bool nodeManager::getIsClientOnline(string t_index){
    return (find(mOnlineClients.begin(), mOnlineClients.end(), t_index) != mOnlineClients.end());
}

vector<string> nodeManager::getOnNodes(){
    return onNodes;
}

vector<string> nodeManager::getOffNodes(){
    return offNodes; //recently turned off Nodes
}

ofVec2f nodeManager::getNodePosition(string index){

    return mNodes[index]->getMeanPos();

}

ofPtr<clamourNode> nodeManager::getNode(string index){

    return mNodes[index];
}

void nodeManager::setNodeDrawType(vector<string> indexes, int dt){

    for(int i = 0; i < indexes.size(); i ++){

        mNodes[indexes[i]]->setDrawType(dt);

    }

}


void nodeManager::setNodeSoundType(vector<string> indexes, string st){

    baseData sd = mSoundDictionary.createSoundData(st);

    for(int i = 0; i < indexes.size(); i ++){

        mNodes[indexes[i]]->setSoundData(sd);

    }

}


void nodeManager::setNodeDrawParam(vector<string> indexes, parameter p){

    for(int i = 0; i < indexes.size(); i ++){

        p.init(mNodes[indexes[i]]->getMeanPos()); //if mapped randomly only reset that parameter
        mNodes[indexes[i]]->getDrawData()->setParameter(p);

    }

}


void nodeManager::setNodeSoundParam(vector<string> indexes, parameter p){

    //sorry could be neater but it's late !

    parameter t = mNodes[indexes[0]]->getSoundData()->getParameter(p.name); //a copy of the original
    p.index = t.index; //copy these over
    p.warp = t.warp;

    for(int i = 0; i < indexes.size(); i ++){

        p.init(mNodes[indexes[i]]->getMeanPos());//if mapped randomly only reset that parameter
        mNodes[indexes[i]]->getSoundData()->setParameter(p);

    }

}

 void nodeManager::setCtrlIndexes(vector<string> clients, int len){

    for(int i =0; i < clients.size(); i++){

        mNodes[clients[i]]->setCtrlIndex(len);
    }

 }


void nodeManager::flagNodesReturn(vector<string> clients){

     for(int i =0; i < clients.size(); i++){

        mNodes[clients[i]]->setIsReturnToOn(true);
    }
}

void nodeManager::flagNodeReturn(string client){

    mNodes[client]->setIsReturnToOn(true);
}
