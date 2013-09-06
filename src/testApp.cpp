#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    ofSetFrameRate(60);
    ofSetVerticalSync(false); //achieves higher frame rates for dual screening
    
    mDisplay = ofxFensterManager::get()->createFenster(0,0,640,480, OF_FULLSCREEN);
    mDisplayListener.setup();
    mDisplay->addListener(&mDisplayListener);
    mDisplay->setFrameRate(60);
    
    mControlListener.setup();
    ofxFensterManager::get()->getPrimaryWindow()->addListener(&mControlListener);
    mControl->setFrameRate(60);
    
	receiver.setup(PORT);
    sender.setup("localhost", OUTPORT);
    
	current_msg_string = 0;
    currentControl = 0;
    
    mNodeManager = ofPtr<nodeManager>(new nodeManager());
    
    mDisplayListener.setNodeManager(mNodeManager);
    mControlListener.setNodeManager(mNodeManager);
    
	
    
}

//--------------------------------------------------------------
void testApp::update(){
    
    //this method is overridden !!!
    
    //all this needs moving to control
    
	// hide old messages
	for(int i = 0; i < NUM_MSG_STRINGS; i++){
		if(timers[i] < ofGetElapsedTimef()){
			msg_strings[i] = "";
		}
	}
    
    cout << "test" << endl;
    
	// check for waiting messages
	while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
        
        vector<string> s;
        s = ofSplitString(m.getAddress(), "/");
        string t_index;
    
        if(s[0] == "node"){
            
            string row = m.getArgAsString(0);
            int seat = m.getArgAsInt32(1);
            t_index = row + "_" + ofToString(seat);
        
        }
        
        cout << t_index <<endl;
        
		// check for mouse moved message
		if(m.getAddress() == "/node/position"){
    
            float x = m.getArgAsFloat(2);
            float y = m.getArgAsFloat(3);
            
            mNodeManager->updateNodePosition(t_index, x, y);
            
        }else if(m.getAddress() == "/node/on"){
            
            float x = m.getArgAsFloat(2);
            float y = m.getArgAsFloat(3);
            
            mNodeManager->switchOnNode(t_index, x, y);
            
            
        }else if(m.getAddress() == "/node/off"){
            
            mNodeManager->switchOffNode(t_index);
            
		}else{
			// unrecognized message: display on the bottom of the screen
			string msg_string;
			msg_string = m.getAddress();
			msg_string += ": ";
			for(int i = 0; i < m.getNumArgs(); i++){
				// get the argument type
				msg_string += m.getArgTypeName(i);
				msg_string += ":";
				// display the argument - make sure we get the right type
				if(m.getArgType(i) == OFXOSC_TYPE_INT32){
					msg_string += ofToString(m.getArgAsInt32(i));
				}
				else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
					msg_string += ofToString(m.getArgAsFloat(i));
				}
				else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
					msg_string += m.getArgAsString(i);
				}
				else{
					msg_string += "unknown";
				}
			}
			// add to the list of strings to display
			msg_strings[current_msg_string] = msg_string;
			timers[current_msg_string] = ofGetElapsedTimef() + 5.0f;
			current_msg_string = (current_msg_string + 1) % NUM_MSG_STRINGS;
			// clear the next line
			msg_strings[current_msg_string] = "";
		}
        
	}
    

    
}


//--------------------------------------------------------------
void testApp::draw(){
    
   //this does nothing now
    

    
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    
    if(key >= 49 && key <= 51){
        
        ofxOscMessage m;
        m.setAddress("/allClients/newControl");
        
        switch(key){
        
            case '1':
                currentControl = 0;
                m.addIntArg(currentControl);
                m.addStringArg("openFrameworks default text");
                mNodeManager->resetNodes();
            break;
                
            case '2':
                currentControl = 1;
                m.addIntArg(currentControl);
            break;
                
            case '3':
                currentControl = 2;
                m.addIntArg(currentControl);
                mNodeManager->resetNodes();
            break;
        
        }

        sender.sendMessage(m);
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
