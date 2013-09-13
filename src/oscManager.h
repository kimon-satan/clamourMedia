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
#define NUM_MSG_STRINGS 6

enum msgType{
    
    CLAMOUR_MSG_METEOR_IN,
    CLAMOUR_MSG_METEOR_OUT,
    CLAMOUR_MSG_SC_OUT

};


class oscManager{

    public:
    
    oscManager();
    void update();
    void setNodeManager(ofPtr<nodeManager> p);

    void setAllClients(int control);
    void setControl(vector<string> clients, string control);
    void setControl(vector<string> clients, string control, string text);
    void setText(vector<string> clients, string text);
    

    //to SC
    
    void sendInit();
    void startSynth(string index);
    void stopSynth(string index);
    void updateSynth(string index);
    
    //getters and setters (internal)
    string getMsgString(int mt);
    
    private:
    
    void logMessages(ofxOscMessage m, int type);
    void addToBundle(string index, ofxOscMessage m);
    
    ofxOscReceiver receiver;
    ofxOscSender sender;
    ofxOscSender SCsender;
    
    map<string, ofxOscBundle> outBundle;
    
    ofPtr<nodeManager> pNodeManager;
    
    vector<vector<string> > msg_strings;
    vector<int> msg_string_count;

};

#endif /* defined(__clamourMedia__oscManager__) */
