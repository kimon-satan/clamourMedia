//
//  nodeManager.cpp
//  clamourMedia
//
//  Created by Simon Katan on 06/09/2013.
//
//

#include "nodeManager.h"

nodeManager::nodeManager(){
    
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

vector<string> nodeManager::getOnNodes(){
    return onNodes;
}

ofVec2f nodeManager::getNodePosition(string index){
    
    cout << index << ":" << mNodes[index]->getMeanPos() << endl;
    return mNodes[index]->getMeanPos();
    
}

ofPtr<clamourNode> nodeManager::getNode(string index){
    
    return mNodes[index];
}

