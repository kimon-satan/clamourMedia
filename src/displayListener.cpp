//
//  displayWindow.cpp
//  clamourMedia
//
//  Created by Simon Katan on 06/09/2013.
//
//

#include "displayListener.h"


void displayListener::setup(){



}

void displayListener::draw(){

   ofSetColor(255);
    
    ofDrawBitmapString(ofToString(ofGetFrameRate(),2), 20,20);
    
    vector<string> onNodes(pNodeManager->getOnNodes());
    
    for(int i = 0; i < onNodes.size(); i ++){
        
        mNodeRenderer.renderNode(pNodeManager->getNode(onNodes[i]));
      
    }
    

}

void displayListener::keyPressed(int key, ofxFenster* window){
    
    
}

void displayListener::setNodeManager(ofPtr<nodeManager> p){
    
    pNodeManager = p;
    
}

void displayListener::setWindowDims(float w, float h){
    
    width = w;
    height = h;
    mNodeRenderer.setScreen((int)w, (int)h);
}