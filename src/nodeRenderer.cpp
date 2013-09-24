//
//  nodeRenderer.cpp
//  clamourMedia
//
//  Created by Simon Katan on 09/09/2013.
//
//

#include "nodeRenderer.h"


nodeRenderer::nodeRenderer(){

    debugFont.loadFont("fonts/Goulong/Goulong.ttf", 15);

}


void nodeRenderer::renderNodes(map<string, ofPtr<clamourNode> > nodes){


    for(map<string, ofPtr<clamourNode> >::iterator it = nodes.begin(); it != nodes.end(); it ++){

        if(it->second->getIsFiring()){

            string dt = it->second->getDrawType();
            if(dt == "DEBUG")
            {
                ofVec2f pos = it->second->getMeanPos_abs();
                pos *= screenData::height;
                debugFont.drawString(it->second->getRow() + "_" + ofToString(it->second->getSeat()), pos.x, pos.y);
            }
            if(dt == "FLICKER")drawFlicker(it->second);
            if(dt == "ROUND")drawRound(it->second);
        }

    }



}


void nodeRenderer::drawFlicker(ofPtr<clamourNode> n){

    ofVec2f pos = n->getMeanPos_abs();
    pos *= screenData::height;

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

    ofVec2f pos = n->getMeanPos_abs();

    pos *= screenData::height;

    std::tr1::shared_ptr<roundDrawData> rdd(dynamic_pointer_cast <roundDrawData> (n->getDrawData()));


    ofSetColor(255 * n->getEnvVal());
    float t_size = rdd->getParameter("size").abs_val;

    ofNoFill();
    ofCircle( pos.x, pos.y, t_size);




}
