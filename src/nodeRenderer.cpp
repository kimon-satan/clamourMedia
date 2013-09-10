//
//  nodeRenderer.cpp
//  clamourMedia
//
//  Created by Simon Katan on 09/09/2013.
//
//

#include "nodeRenderer.h"


nodeRenderer::nodeRenderer(){

    screenWidth = 1280;
    screenHeight = 800;

    
}

void nodeRenderer::setScreen(int w, int h){

    screenWidth = w;
    screenHeight = h;
    
}

void nodeRenderer::renderNode(ofPtr<clamourNode> n){

    ofVec2f pos = n->getMeanPos();
    pos *= ofVec2f(screenWidth, screenHeight);
    
    
    switch(n->getDrawType()){
            
        case CLAMOUR_DRAW_DEBUG:
            ofDrawBitmapString(n->getRow() + "_" + ofToString(n->getSeat()), pos);
            break;
        
        case CLAMOUR_DRAW_FLICKER:
            drawFlicker(n);
            break;
            
    }
    

}


void nodeRenderer::drawFlicker(ofPtr<clamourNode> n){
    
    ofVec2f pos = n->getMeanPos();
    pos *= ofVec2f(screenWidth, screenHeight);
    
    std::tr1::shared_ptr<flickerDrawData> fdd(dynamic_pointer_cast <flickerDrawData> (n->getDrawData()));

    
    ofSetColor(255);
    if(ofRandom(1) < fdd->getParameter("flicker").abs_val){
    
        glBegin(GL_POINTS);
        glColor3ub(255,255,255);
        
        float size = fdd->getParameter("size").abs_val;
        
        for(int i = 0; i < 50; i ++){
            glVertex2d(pos.x + ofRandom(-size/2,size/2), pos.y + ofRandom(-size/2,size/2));
        }
        glEnd();
    }

}