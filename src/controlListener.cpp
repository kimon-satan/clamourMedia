//
//  controlListener.cpp
//  clamourMedia
//
//  Created by Simon Katan on 06/09/2013.
//
//

#include "controlListener.h"

void controlListener::setup(){

    mOscManager = ofPtr<oscManager>(new oscManager());
    mNodeManager = ofPtr<nodeManager>(new nodeManager());
    mOscManager->setNodeManager(mNodeManager);
    
    mOscManager->sendInit();
    
    gui = new ofxUICanvas(0,0,ofGetWidth(),ofGetHeight());
	gui->addWidgetDown(new ofxUILabel("CLAMOUR CONTROL PANEL", OFX_UI_FONT_LARGE));
    gui->addWidgetDown(new ofxUIToggle("test tog", false, 20, 20));
    ofAddListener(gui->newGUIEvent,this,&controlListener::guiEvent);
    

}


void controlListener::update(ofxFenster *f){

    mOscManager->update();
    mNodeManager->updateNodes();

}

void controlListener::draw(){

    ofBackground(100);
    ofSetColor(255);
    
    gui->draw();
    
   /*  string buf;
     buf = "listening for osc messages on port: " + ofToString(METEOR_IN_PORT);
     ofDrawBitmapString(buf, 10, 20);
     ofDrawBitmapString("currentControl: " + ofToString(currentControl,0), 10, 40);
    
     vector<string> msgs = mOscManager->getMsgStrings();
     
     for(int i = 0; i < msgs.size(); i++){
         ofDrawBitmapString(msgs[i], 10, 40 + 15 * i);
     };*/
     
    

}

ofPtr<nodeManager> controlListener::getNodeManager(){

    return mNodeManager;
}

void controlListener::setDisplayRef (ofxFenster * f){

    displayFenster = f;
}

void controlListener::keyPressed(int key, ofxFenster* window){
    
    
    if(key >= 49 && key <= 51){
     
        mOscManager->setAllClients(key - 49);
        mNodeManager->resetNodes();
   
    }

    
}

void controlListener::guiEvent(ofxUIEventArgs &e){

    cout << "guiEvent \n";

}



