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


void nodeManager::updateNodes(){
    
    for(int i = 0; i < onNodes.size(); i ++){
        
        string t_index = onNodes[i];
        mNodes[t_index]->updateHistory();
        mNodes[t_index]->updateDrawData();
        mNodes[t_index]->updateSoundData();
        
    }
    
}


void nodeManager::resetNodes(){
    
    //turn off all current nodes
    for(int i = 0; i < onNodes.size(); i ++){
        
        string t_index = onNodes[i];
        mNodes[t_index]->setIsOn(false);
        
    }
    
    onNodes.clear();
    
    
}

void nodeManager::switchOnNode(string t_index, float x, float y){
    
    mNodes[t_index]->setIsOn(true);
    mNodes[t_index]->setPosition(ofVec2f(x, y));
    onNodes.push_back(t_index);
    
}

void nodeManager::switchOffNode(string t_index){
    
    vector<string>::iterator it = remove(onNodes.begin(), onNodes.end(), t_index);
    
    if(it != onNodes.end()){
        onNodes.erase(it);
    }
    
    
    
    mNodes[t_index]->setIsOn(false);
    mNodes[t_index]->clearHistory();
    
}



void nodeManager::updateNodePosition(string t_index, float x, float y){
    
    mNodes[t_index]->setPosition(ofVec2f(x,y));
    
}

void nodeManager::updateOnlineClients(vector<string> v){mOnlineClients = v;}
vector<string> nodeManager::getOnlineClients(){return mOnlineClients;}

vector<string> nodeManager::getOnNodes(){
    return onNodes;
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

