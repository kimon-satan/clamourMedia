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
#include  "scMessenger.h""

#include "nodeManager.h"
#include "clientManager.h"
#include "zoneManager.h"
#include "splashManager.h"

#define METEOR_IN_PORT 41234
#define METEOR_OUT_PORT 42345



struct cBundle{

    cBundle(){
        sendCount = 0;
    }

    ofxOscBundle bundle;
    int sendCount;
    int sendNum;

};

enum msgType{

    CLAMOUR_MSG_METEOR_IN,
    CLAMOUR_MSG_METEOR_OUT,
    CLAMOUR_MSG_SC_OUT

};


class oscManager{

    public:

    oscManager();
    void updateInMessages();
    void updateOutMessages();
    void sendBundle();

    void setNodeManager(ofPtr<nodeManager> p);
    void setClientManager(ofPtr<clientManager> p);
    void setZoneManager(ofPtr<zoneManager> p);
    void setSplashManager(ofPtr<splashManager> p);

    void setControl(vector<string> clients, string control);
    void setControl(vector<string> clients, string control, string text);

    void setText(vector<string> clients, string text);


    //to SC

    void sendInit();

    void stopSynth(ofPtr<baseZode> n);
    void updateSynth(ofPtr<baseZode> n);

    void startSynth(ofPtr<baseZode> z);
    void startSynth(string z_name, clamourEvent & e, baseData & sd);

    //getters and setters (internal)
    string getMsgString(int mt);

    static ofxOscSender SCsender;

    private:

    void logMessages(ofxOscMessage m, int type);
    void addToBundle(string index, ofxOscMessage m);

    ofxOscReceiver receiver;
    ofxOscSender sender;


    map<string, cBundle> outBundle;

    ofPtr<nodeManager> pNodeManager;
    ofPtr<clientManager> pClientManager;
    ofPtr<zoneManager> pZoneManager;
    ofPtr<splashManager> pSplashManager;

    vector<vector<string> > msg_strings;
    vector<int> msg_string_count;

};

#endif /* defined(__clamourMedia__oscManager__) */
