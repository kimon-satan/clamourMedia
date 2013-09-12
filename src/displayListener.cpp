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

  /*  ofSetColor(255);
    
    ofDrawBitmapString(ofToString(ofGetFrameRate(),2), 20,20);
    
    if(mWindow){
        mNodeRenderer.setScreen(mWindow->getWidth(), mWindow->getHeight());
    }
    
    vector<string> onNodes(pNodeManager->getOnNodes());
    
    for(int i = 0; i < onNodes.size(); i ++){
        
        mNodeRenderer.renderNode(pNodeManager->getNode(onNodes[i]));
      
    }*/
    

}

void displayListener::keyPressed(int key, ofxFenster* window){
    
    mWindow = window;
    
}

void displayListener::setNodeManager(ofPtr<nodeManager> p){
    
    pNodeManager = p;
    
}