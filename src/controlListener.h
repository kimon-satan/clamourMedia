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

class controlListener: public baseFensterListener{


public:
    
    
    void setup();
    void update(ofxFenster * f);
    void draw();
    
    void keyPressed(int key, ofxFenster* window);
    void setDisplayRef(ofxFenster * f);
    
    
    ofPtr<nodeManager> getNodeManager();
    
    ofxUICanvas *gui;
	void guiEvent(ofxUIEventArgs &e);
    
private:
    
      ofPtr<oscManager> mOscManager;
      ofPtr<nodeManager> mNodeManager;
    
      ofxFenster * displayFenster;
    
      int currentControl;

};

#endif /* defined(__clamourMedia__controlListener__) */
