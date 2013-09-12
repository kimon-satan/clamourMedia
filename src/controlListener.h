//
//  controlListener.h
//  clamourMedia
//
//  Created by Simon Katan on 06/09/2013.
//
//

#ifndef __clamourMedia__controlListener__
#define __clamourMedia__controlListener__

#include "oscManager.h"
#include "ofxUI.h"
#include "game.h"
#include "ofxXmlSettings.h"


class controlListener{
    
    //this basically replaces the testApp
    
public:
    
    void setup();

    void update();
    void draw();
    
    ofPtr<nodeManager> getNodeManager();

    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    

    
private:
    
    void setupGUI();
    void setupTextArea(ofxUIWidget * w);
    void updateGUIElements();
    
    void setupGames();
    void implementStage();

	void guiEvent(ofxUIEventArgs &e);

    
    
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
    int width, height;
    
};

#endif /* defined(__clamourMedia__controlListener__) */
