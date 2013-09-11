#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    ofSetFrameRate(60);
    ofSetVerticalSync(false); //achieves higher frame rates for dual screening
    ofEnableSmoothing();
    
    mDisplay = ofxFensterManager::get()->createFenster(0,0,640,480, OF_WINDOW);
    mDisplayListener.setup();
    mDisplay->addListener(&mDisplayListener);
    mDisplay->setFrameRate(60);
    
    
    mControl =  ofxFensterManager::get()->getPrimaryWindow();
    mControl->addListener(&mControlListener);
    mControlListener.setup(mControl);
    mControl->setFrameRate(60);
    
    
    mDisplayListener.setNodeManager(mControlListener.getNodeManager());
    
    
}

//--------------------------------------------------------------
void testApp::update(){
    
    //this method is overridden !!!
    
}


//--------------------------------------------------------------
void testApp::draw(){
    
   //this does nothing now
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    //eventually move this to UI element
    if(key == 'F'){
        mDisplay->toggleFullscreen();
    }
    
        
}


//--------------------------------------------------------------
void testApp::keyReleased(int key){
    

    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    
}

void testApp::exit(){

    

}
