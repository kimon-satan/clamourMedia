//
//  nodeRenderer.h
//  clamourMedia
//
//  Created by Simon Katan on 09/09/2013.
//
//

#ifndef __clamourMedia__nodeRenderer__
#define __clamourMedia__nodeRenderer__

#include "clamourNode.h"

class nodeRenderer{

    
    public:
    
    nodeRenderer();
    
    void renderNode(ofPtr<clamourNode> n);
    void setScreen(int w, int h);
    
    //drawing types
    void drawFlicker(ofPtr<clamourNode> n);
    
    private:
    
    int screenWidth, screenHeight;
 
    


};

#endif /* defined(__clamourMedia__nodeRenderer__) */
