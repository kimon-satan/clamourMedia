//
//  nodeRenderer.cpp
//  clamourMedia
//
//  Created by Simon Katan on 09/09/2013.
//
//

#include "nodeRenderer.h"


nodeRenderer::nodeRenderer() {

    debugFont.loadFont("fonts/Goulong/Goulong.ttf", 15);

}


void nodeRenderer::renderNodes(map<string, ofPtr<clamourNode> > nodes) {


    for(map<string, ofPtr<clamourNode> >::iterator it = nodes.begin(); it != nodes.end(); it ++) {

        if(!it->second->getIsSleeping()) {

            baseData bd = it->second->getDrawData();

            if(bd.getName() == "DEBUG") {
                ofVec2f pos = it->second->getMeanPos_abs();
                pos *= screenData::height;
                debugFont.drawString(it->second->getRow() + "_" + ofToString(it->second->getSeat()), pos.x, pos.y);
            }
            if(bd.getName() == "FLICKER")drawFlicker(it->second, bd);
            if(bd.getName() == "ROUND")drawRound(it->second, bd);
            if(bd.getName() == "mousePointer")drawPointer(it->second, bd);
        }

    }



}


void nodeRenderer::drawFlicker(ofPtr<clamourNode> n, baseData &bd) {

    ofVec2f pos = n->getMeanPos_abs();
    pos *= screenData::height;


    ofSetColor(255);


    if(ofRandom(1) < bd.getParameter("flicker").abs_val) {

        glBegin(GL_POINTS);
        // glColor3ub(255 * n->getEnvVal() ,255 * n->getEnvVal(),255 * n->getEnvVal());
        glColor3ub(255,255,255);

        float mul = (n->getIsFiring())? 0.5 : 1.0;
        float size = bd.getParameter("size").abs_val * mul;
        int numPs = (n->getIsFiring())? 200 : 50;

        for(int i = 0; i < 50; i ++) {
            glVertex2d(pos.x + ofRandom(-size/2,size/2), pos.y + ofRandom(-size/2,size/2));
        }
        glEnd();
    }

}


void nodeRenderer::drawRound(ofPtr<clamourNode> n, baseData &bd) {

    ofVec2f pos = n->getMeanPos_abs();
    pos *= screenData::height;

    ofSetColor(255 * n->getEnvVal());
    float t_size = bd.getParameter("size").abs_val;

    ofNoFill();
    ofCircle( pos.x, pos.y, t_size);

}

void nodeRenderer::drawPointer(ofPtr<clamourNode> n, baseData &bd) {

    ofPath p = n->getEdgeTemplate();
    ofVec2f pos = n->getMeanPos_abs();
    pos *= screenData::height;


    ofPushMatrix();

    ofTranslate(pos.x, pos.y, 0);
    ofScale(screenData::height, screenData::height,1.0);

    p.setFilled(true);
    p.setColor(255);
    p.draw();

    p.setFilled(false);
    p.setStrokeColor(0);
    p.draw();

    ofPopMatrix();

}
