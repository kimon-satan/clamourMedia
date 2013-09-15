//
//  oscManager.cpp
//  clamourMedia
//
//  Created by Simon Katan on 07/09/2013.
//
//

#include "oscManager.h"


oscManager::oscManager(){

    receiver.setup(METEOR_IN_PORT);
    sender.setup("localhost", METEOR_OUT_PORT);
    SCsender.setup("localhost", SC_OUT_PORT);

    for(int i = 0; i < 3; i ++){
        vector<string> s;
        msg_strings.push_back(s);
        msg_string_count.push_back(0);
    }
    
}

void oscManager::update(){
    
    for(int i = 0; i < 3; i ++){
        msg_string_count[i] = 0;
    }
    
	// check for waiting messages
	while(receiver.hasWaitingMessages()){
        
		ofxOscMessage m;
		receiver.getNextMessage(&m);
        
        logMessages(m, CLAMOUR_MSG_METEOR_IN);
        
        vector<string> s;
        s = ofSplitString(m.getAddress(), "/");
        string t_index;

        
        if(s[1] == "node"){
            
            string row = m.getArgAsString(0);
            int seat = (int)m.getArgAsFloat(1);
            t_index = row + "_" + ofToString(seat);
            
        }
        
		if(m.getAddress() == "/node/position"){
            
            float x = m.getArgAsFloat(2);
            float y = m.getArgAsFloat(3);
            
            /*if(pNodeManager->getNode(t_index)->getIsOn()){
                pNodeManager->updateNodePosition(t_index, x, y);
                updateSynth(t_index);
            }*/
            
            pNodeManager->shiftNodePosition(t_index, x, y);
            
        }else if(m.getAddress() == "/node/on"){
            
            float x = m.getArgAsFloat(2);
            float y = m.getArgAsFloat(3);
            
            //pNodeManager->switchOnNode(t_index, x, y);
            pNodeManager->switchOnNode(t_index);
            pNodeManager->beginShift(t_index, x, y);
            startSynth(t_index);
            
        }else if(m.getAddress() == "/node/off"){
            
            pNodeManager->switchOffNode(t_index);
            stopSynth(t_index);
            
		}else if(m.getAddress() == "/update/onlineUsers"){
            
            vector<string> v;
            
            for(int i = 0; i < m.getNumArgs(); i++){
                v.push_back(m.getArgAsString(i));
            }
            
            pNodeManager->updateOnlineClients(v);
        
        }
        
	}
    
    //send the client's outbundle to Meteor if it has messages waiting
    
    map<string, ofxOscBundle>::iterator it;
    
    for(it = outBundle.begin(); it != outBundle.end(); it++){
        
        if(it->second.getMessageCount() > 0){
            sender.sendBundle(it->second);
            it->second.clear();
        }
        
    }
    
 


}


void oscManager::logMessages(ofxOscMessage m, int mt){

    if(msg_string_count[mt] >= NUM_MSG_STRINGS)return; //for when there's loads of messages
    
    string msg_string;
    msg_string = m.getAddress();
    msg_string += ": ";
    
    for(int i = 0; i < m.getNumArgs(); i++){
        
        
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
        
        msg_string += ", ";
    }
    
    // add to the list of strings to the log
    msg_strings[mt].push_back( msg_string.substr(0,50)); //maximum 50 chars
    msg_string_count[mt] += 1;
   
    
    //make space for new messages
    if(msg_strings[mt].size() > NUM_MSG_STRINGS)msg_strings[mt].erase(msg_strings[mt].begin());

}





void oscManager::setNodeManager(ofPtr<nodeManager> p){pNodeManager = p;}

string oscManager::getMsgString(int mt){
    
    string s;
    
    for(int i = 0; i < msg_strings[mt].size(); i++){
        
        s += msg_strings[mt][i];
        s += "\n\n";
        
    }
    
    return s;

}

//--------------------------Meteor Messages-----------------------------//

//messages to Meteor need to bundled because of concurrency

void oscManager::setAllClients(int control){
    
    
    ofxOscMessage m;
    m.setAddress("/allClients/newControl");
    m.addIntArg(control);
    
    if(control == 0)m.addStringArg("default text");
    
    logMessages(m, CLAMOUR_MSG_METEOR_OUT);
    
    sender.sendMessage(m);

}

void oscManager::setControl(vector<string> clients, string control){

    
    for(int i = 0; i < clients.size(); i++){
        
        ofxOscMessage m;
        
        m.setAddress("/newControl");
        m.addStringArg(clients[i]);
        m.addStringArg(control);
        
        logMessages(m, CLAMOUR_MSG_METEOR_OUT);
        
        addToBundle(clients[i], m);
    
    }

}

void oscManager::setControl(vector<string> clients, string control, string text){
    
    
    for(int i = 0; i < clients.size(); i++){
        
        ofxOscMessage m;
        
        m.setAddress("/newControl");
        m.addStringArg(clients[i]);
        m.addStringArg(control);
        m.addStringArg(text);
        
        logMessages(m, CLAMOUR_MSG_METEOR_OUT);
        
        addToBundle(clients[i], m);
        
    }
    
}

void oscManager::setText(vector<string> clients, string text){
    
    
    for(int i = 0; i < clients.size(); i++){
        ofxOscMessage m;
        
        m.setAddress("/newText");
        m.addStringArg(clients[i]);
        m.addStringArg(text);
        
        logMessages(m, CLAMOUR_MSG_METEOR_OUT);
        
        addToBundle(clients[i], m);
       
    }
    
  
    
}

void oscManager::addToBundle(string index, ofxOscMessage m){

    if(outBundle.find(index) != outBundle.end()){
        outBundle[index].addMessage(m);
    }else{
        ofxOscBundle b;
        outBundle[index] = b;
        outBundle[index].addMessage(m);
    }

}





//------------------------------SuperCollider Messages---------------------------------//

void oscManager::sendInit(){

    ofxOscMessage m;
    m.setAddress("/init");
    SCsender.sendMessage(m);
    
    logMessages(m, CLAMOUR_MSG_SC_OUT);

}

void oscManager::startSynth(string index){
    
    ofPtr<baseData> sd = pNodeManager->getNode(index)->getSoundData();
    
    ofxOscMessage m;
    m.setAddress("/startSynth");
    m.addStringArg(index);
    
    m.addStringArg(sd->getName());
    
    vector<float> vals = sd->getAbsVals();
    
    for(int i = 0; i < vals.size(); i++){
        m.addFloatArg(vals[i]);
    }
    
    SCsender.sendMessage(m);
    logMessages(m, CLAMOUR_MSG_SC_OUT);
    
}

void oscManager::updateSynth(string index){
    
   ofVec2f pos(pNodeManager->getNodePosition(index));
    
    ofxOscMessage m;
    m.setAddress("/updateSynth");
    m.addStringArg(index);
    
    ofPtr<baseData> sd = pNodeManager->getNode(index)->getSoundData();
    
    m.addStringArg(sd->getName()); //add the name to keep the indexing th same for update and start at the SC end
    
    vector<float> vals = sd->getAbsVals();
    
    for(int i = 0; i < vals.size(); i++){
        m.addFloatArg(vals[i]);
    }
    
    SCsender.sendMessage(m);
    
    logMessages(m, CLAMOUR_MSG_SC_OUT);
    
    
}

void oscManager::stopSynth(string index){
    
    ofxOscMessage m;
    m.setAddress("/stopSynth");
    m.addStringArg(index);
    SCsender.sendMessage(m);
    
    logMessages(m, CLAMOUR_MSG_SC_OUT);

}