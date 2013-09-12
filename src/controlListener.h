//
//  controlListener.h
//  clamourMedia
//
//  Created by Simon Katan on 06/09/2013.
//
//

#ifndef __clamourMedia__controlListener__
#define __clamourMedia__controlListener__

#include "baseFensterListener.h"
#include "oscManager.h"
#include "ofxUI.h"
#include "game.h"
#include "ofxXmlSettings.h"


class controlListener: public baseFensterListener{
    
    //this basically replaces the testApp
    
public:
    
    void setup();

    void update(ofxFenster * f);
    void draw();
    
    void setDisplayRef(ofxFenster * f);
    
    ofPtr<nodeManager> getNodeManager();

    

    
private:
    
    void setupGUI();
    void setupTextArea(ofxUIWidget * w);
    void updateGUIElements();
    
    void setupGames();
    void implementStage();

	void guiEvent(ofxUIEventArgs &e);
    void keyPressed(int key, ofxFenster* window);
    void mousePressed(int x, int y, int button, ofxFenster * window);
    void mouseReleased(int x, int y, int button, ofxFenster * window);
    
    
    ofxUICanvas *gui;
    ofxUIFont * uiFont;
    
    ofPtr<oscManager> mOscManager;
    ofPtr<nodeManager> mNodeManager;
    
    vector<string> mPlayerIndexes;
    map<string, ofPtr<group> > mGroups;
    vector<ofPtr<game> > mGames;
    
    
    ofPtr<game> mCurrentGame;
    int mGameBrowseIndex;
    ofxFenster * displayFenster;
    
    bool isMouseDown;
    
    int currentControl;
    
};

#endif /* defined(__clamourMedia__controlListener__) */
