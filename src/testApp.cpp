#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    ofSetFrameRate(60);
	// listen on the given port
	cout << "listening for osc messages on port " << PORT << "\n";
	receiver.setup(PORT);
    sender.setup("localhost", OUTPORT);
    
	current_msg_string = 0;
    currentControl = 0;


    //populate nodeArray
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            
            string t_index = ofToString(char(65 + j)) + "_" + ofToString(i + 1);
            clamourNode t_mn(i + 1, ofToString(char(65 + j)));
            mNodes[t_index] = t_mn;
            
        }
    }
    
    
	
    
}

//--------------------------------------------------------------
void testApp::update(){
    
	// hide old messages
	for(int i = 0; i < NUM_MSG_STRINGS; i++){
		if(timers[i] < ofGetElapsedTimef()){
			msg_strings[i] = "";
		}
	}
    
	// check for waiting messages
	while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
        
		// check for mouse moved message
		if(m.getAddress() == "/node/position"){
    
            
            string row = m.getArgAsString(0);
            int seat = m.getArgAsInt32(1);
            float x = m.getArgAsFloat(2);
            float y = m.getArgAsFloat(3);
            
            cout << row << ":" << seat << ":" << x << ":" << y << ":" << endl;
            
            string t_index = row + "_" + ofToString(seat);
            
            mNodes[t_index].setPosition(ofVec2f(x,y));
            
        }else if(m.getAddress() == "/node/on"){
            
            string row = m.getArgAsString(0);
            int seat = m.getArgAsInt32(1);
            float x = m.getArgAsFloat(2);
            float y = m.getArgAsFloat(3);
            string t_index = row + "_" + ofToString(seat);
            
            mNodes[t_index].setIsOn(true);
            mNodes[t_index].setPosition(ofVec2f(x, y));
            onNodes.push_back(t_index);
            
            
        }else if(m.getAddress() == "/node/off"){
            
            string row = m.getArgAsString(0);
            int seat = m.getArgAsInt32(1);
            string t_index = row + "_" + ofToString(seat);
            
            vector<string>::iterator it = remove(onNodes.begin(), onNodes.end(), t_index);
            onNodes.erase(it);
            
            mNodes[t_index].setIsOn(false);
            mNodes[t_index].clearHistory();

            
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
    
    for(int i = 0; i < onNodes.size(); i ++){
        
        string t_index = onNodes[i];
        mNodes[t_index].updateHistory();
        
    }

    
}


//--------------------------------------------------------------
void testApp::draw(){
    
    ofBackground(0);
    
    ofSetColor(255);
	string buf;
	buf = "listening for osc messages on port: " + ofToString(PORT);
	ofDrawBitmapString(buf, 10, 20);
    ofDrawBitmapString("currentControl: " + ofToString(currentControl,0), 10, 40);
    
    
	for(int i = 0; i < NUM_MSG_STRINGS; i++){
		ofDrawBitmapString(msg_strings[i], 10, 40 + 15 * i);
	}
    
    
    for(int i = 0; i < onNodes.size(); i ++){
    
        string t_index = onNodes[i];
        ofDrawBitmapString(t_index, mNodes[t_index].getMeanPos());
        
    }
    
    
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
                resetNodes();
            break;
                
            case '2':
                currentControl = 1;
                m.addIntArg(currentControl);
            break;
                
            case '3':
                currentControl = 2;
                m.addIntArg(currentControl);
                resetNodes();
            break;
        
        }

        sender.sendMessage(m);
    }
    
}


void testApp::resetNodes(){
    
    //turn off all current nodes
    for(int i = 0; i < onNodes.size(); i ++){
    
        string t_index = onNodes[i];
        mNodes[t_index].setIsOn(false);
    
    }
    
    onNodes.clear();

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
