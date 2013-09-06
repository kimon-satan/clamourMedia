#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "clamourNode.h"

// listen on port 12345
#define PORT 41234
#define OUTPORT 42345
#define NUM_MSG_STRINGS 20


class testApp : public ofBaseApp {
public:
    
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void exit();
    
    void resetNodes();
    
    ofTrueTypeFont font;
    ofxOscReceiver receiver;
    ofxOscSender sender;
    
    int currentControl;
    int current_msg_string;
    string msg_strings[NUM_MSG_STRINGS];
    float timers[NUM_MSG_STRINGS];
    
    map<string, clamourNode> mNodes;
    vector<string> onNodes;
    

    
};
