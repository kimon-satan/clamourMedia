#pragma once

#include "clamourUtils.h"

#include "ofMain.h"
#include "displayWindow.h"
#include "ofxOsc.h"
#include "ofxUI.h"

#include "oscManager.h"
#include "xmlLoader.h"



class ofApp : public ofBaseApp
{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

private:

    void setupGUI();
    void setupTextArea(ofxUIWidget * w);
    void updateGUIElements();

    void loadXML();

    void implementStage();
    void unpackClients(vector<string> &clients, command &cmd);

    void guiEvent(ofxUIEventArgs &e);

    void resetEverything();

    displayWindow mDisplay;

    ofxUICanvas *gui;

    ofPtr<clientManager> mClientManager;

    ofPtr<oscManager> mOscManager;
    ofPtr<nodeManager> mNodeManager;
    ofPtr<zoneManager> mZoneManager;
    ofPtr<splashManager> mSplashManager;

    vector<string> mPlayerIndexes;
    vector<ofPtr<game> > mGames;

    ofPtr<game> mCurrentGame;

    map<string, clamourNode> mNodePresets; //may ultimately need to go somewhere else (i.e. nodeManager);
    int mGameBrowseIndex;

    bool isMouseDown;

    int currentControl;
};
