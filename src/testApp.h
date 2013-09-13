#pragma once

#include "ofxFensterManager.h"
#include "ofxXmlSettings.h"
#include "ofxUI.h"

#include "displayListener.h"
#include "oscManager.h"
#include "game.h"


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
    
    
private:
    
    void setupGUI();
    void setupTextArea(ofxUIWidget * w);
    void updateGUIElements();
    
    void loadXML();
    void loadCommands(ofPtr<game> gm, ofxXmlSettings &XML, int stage, int pty = 0);
    void parseActions(command &cmd, ofxXmlSettings &XML);
    void implementStage();
    
	void guiEvent(ofxUIEventArgs &e);
    
    ofxFenster * mDisplay;
    displayListener * mDisplayListener;
    
    ofxUICanvas *gui;
    ofxUIFont * uiFont;
    
    ofPtr<oscManager> mOscManager;
    ofPtr<nodeManager> mNodeManager;
    
    vector<string> mPlayerIndexes;
    map<string, ofPtr<group> > mGroups;
    vector<ofPtr<game> > mGames;
    
    
    ofPtr<game> mCurrentGame;
    int mGameBrowseIndex;
    
    bool isMouseDown;
    
    int currentControl;
    
    
    
};
