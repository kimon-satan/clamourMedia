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

class displayListener: public baseFensterListener {

    public:
    
    void setup();
    void draw();
   
    void setNodeManager(ofPtr<nodeManager> p);
    void keyPressed(int key, ofxFenster* window);
    void setWindowDims(float width, float height);
    
    private:
    
    ofPtr<nodeManager> pNodeManager;
    nodeRenderer mNodeRenderer;

};

#endif /* defined(__clamourMedia__displayWindow__) */
