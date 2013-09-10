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
    
    void setupGames();
    void implementStage();
    void setControl(group g);
    void setText(group g);

	void guiEvent(ofxUIEventArgs &e);
    void keyPressed(int key, ofxFenster* window);
    void mousePressed(int x, int y, int button, ofxFenster * window);
    void mouseReleased(int x, int y, int button, ofxFenster * window);
    
    ofxUICanvas *gui;
    
    ofPtr<oscManager> mOscManager;
    ofPtr<nodeManager> mNodeManager;
    
    vector<string> mPlayerIndexes;
    vector<ofPtr<group> > mGroups; //this is added instead of in games
    vector<ofPtr<game> > mGames;
    
    
    ofPtr<game> mCurrentGame;
    ofxFenster * displayFenster;
    
    bool isMouseDown;
    
    int currentControl;
    
};

#endif /* defined(__clamourMedia__controlListener__) */
