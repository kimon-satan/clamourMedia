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

void displayListener::update(){

    mTitleRenderer.update();
}

void displayListener::draw(){

   ofSetColor(255);
    
    
    ofDrawBitmapString(ofToString(ofGetFrameRate(),2), 20,20);
    
    vector<string> onNodes(pNodeManager->getOnNodes());
    
    for(int i = 0; i < onNodes.size(); i ++){
        
        mNodeRenderer.renderNode(pNodeManager->getNode(onNodes[i]));
      
    }
    
    mTitleRenderer.draw();
    

}

void displayListener::keyPressed(int key, ofxFenster* window){
   
    //eventually move this to UI element
    if(key == 'F'){
         mNodeRenderer.setScreen(ofGetScreenWidth(), ofGetScreenHeight());
        mTitleRenderer.setScreen(ofGetScreenWidth(), ofGetScreenHeight());
        ofToggleFullscreen();
    }
    
    
}

void displayListener::setNodeManager(ofPtr<nodeManager> p){
    
    pNodeManager = p;
    
}

void displayListener::addTitle(string s, title t){
    
    mTitleRenderer.addTitle(s, t);
}

void displayListener::endTitle(string s){

    mTitleRenderer.endTitle(s);
}

void displayListener::reset(){
    
    mTitleRenderer.reset();
}

