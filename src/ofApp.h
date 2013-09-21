#pragma once

#include "clamourUtils.h"

#include "ofMain.h"
#include "displayWindow.h"
#include "ofxOsc.h"
#include "ofxUI.h"
#include "ofxXmlSettings.h"

#include "oscManager.h"
#include "game.h"
#include "clientManager.h"




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
    void loadCommands(ofPtr<game> gm, ofxXmlSettings &XML, int stage, int pty = 0);
    void parseActions(command &cmd, ofxXmlSettings &XML);
    void selectClients(vector<string> selectors, ofPtr<group> grp);
    void implementStage();

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
    int mGameBrowseIndex;

    bool isMouseDown;

    int currentControl;
};
