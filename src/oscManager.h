//
//  oscManager.h
//  clamourMedia
//
//  Created by Simon Katan on 07/09/2013.
//
//

#ifndef __clamourMedia__oscManager__
#define __clamourMedia__oscManager__

#include "ofxOsc.h"
#include "nodeManager.h"

#define METEOR_IN_PORT 41234
#define METEOR_OUT_PORT 42345
#define SC_OUT_PORT 57120
#define NUM_MSG_STRINGS 20

class oscManager{

    public:
    
    oscManager();
    void update();
    void setNodeManager(ofPtr<nodeManager> p);
    vector<string> getMsgStrings();

    void setAllClients(int control);
    void setControl(vector<string> clients, int control);
    
    //to SC
    
    void sendInit();
    void startSynth(string index);
    void stopSynth(string index);
    void updateSynth(string index);
    
    
    private:
    
    void logMessages(ofxOscMessage m);
    
    ofxOscReceiver receiver;
    ofxOscSender sender;
    ofxOscSender SCsender;
    
    ofPtr<nodeManager> pNodeManager;
    
    vector<string> msg_strings;
    

};

#endif /* defined(__clamourMedia__oscManager__) */
