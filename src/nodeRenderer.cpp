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
    debugFont.loadFont("fonts/Goulong/Goulong.ttf", 15);

}

void nodeRenderer::setScreen(int w, int h){

    screenWidth = w;
    screenHeight = h;

}

void nodeRenderer::renderNodes(vector< ofPtr<clamourNode> > nodes){


    for(vector<ofPtr<clamourNode> >::iterator it = nodes.begin(); it != nodes.end(); it ++){

    string dt = (*it)->getDrawType();


    if(dt == "DEBUG"){
            ofVec2f pos = (*it)->getMeanPos();
            pos *= screenHeight;
            debugFont.drawString((*it)->getRow() + "_" + ofToString((*it)->getSeat()), pos.x, pos.y);
    }
    if(dt == "FLICKER")drawFlicker(*it);
    if(dt == "ROUND")drawRound(*it);

    }



}


void nodeRenderer::drawFlicker(ofPtr<clamourNode> n){

    ofVec2f pos = n->getMeanPos();
    pos *= screenHeight;

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


void nodeRenderer::drawRound(ofPtr<clamourNode> n){

    ofVec2f pos = n->getMeanPos();
    pos *= screenHeight;

    std::tr1::shared_ptr<roundDrawData> rdd(dynamic_pointer_cast <roundDrawData> (n->getDrawData()));


    ofSetColor(255);
    float size = rdd->getParameter("size").abs_val;
    ofNoFill();
    ofCircle( pos.x, pos.y, size );



}
