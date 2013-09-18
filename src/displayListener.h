//
//  displayWindow.h
//  clamourMedia
//
//  Created by Simon Katan on 06/09/2013.
//
//

#ifndef __clamourMedia__displayWindow__
#define __clamourMedia__displayWindow__

#include "baseFensterListener.h"
#include "nodeRenderer.h"
#include "titleRenderer.h"

class displayListener: public baseFensterListener {

    public:
    
    void setup();
    void update();
    void draw();
   
    void setNodeManager(ofPtr<nodeManager> p);
    void keyPressed(int key, ofxFenster* window);

    void addTitle(string s, title t);
    void endTitle(string s);
    
    void reset();
    
    
    private:
    
    ofPtr<nodeManager> pNodeManager;
    nodeRenderer mNodeRenderer;
    titleRenderer mTitleRenderer;

};

#endif /* defined(__clamourMedia__displayWindow__) */
