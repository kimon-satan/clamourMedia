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

            if(bd.getName() == "simplePointer")simplePointer(it->second, bd);
            if(bd.getName() == "fanPointer")fanPointer(it->second, bd);
        }

    }



}




void nodeRenderer::simplePointer(ofPtr<clamourNode> n, baseData &bd) {

    if(bd.getParameter("flickerOn").abs_val > 0.01 && ofRandomf() < bd.getParameter("flicker").abs_val)return;

    ofPath p = n->getEdgeTemplate();

    ofPushMatrix();

    ofScale(screenData::height, screenData::height,1.0);
    ofTranslate(n->getMeanPos_abs());
    if(n->getIsRotate())ofRotate(n->getAvRot());

    float h = bd.getParameter("colH").abs_val * 255;
    float s = bd.getParameter("colS").abs_val * 255;
    float bF = bd.getParameter("bFire").abs_val * 255;
    float b = bd.getParameter("bOff").abs_val * 255;
    int pulse = bd.getParameter("pulse").abs_val * ofGetFrameRate();
    ofColor c;

    if(pulse > 0 &&  bd.getParameter("pulseOn").abs_val > 0.01){
        (ofGetFrameNum()%pulse == 0) ? c.setHsb(h,s,bF) : c.setHsb(h,s,b);
    }else{
        c.setHsb(h,s,b);
    }

    float sh = bd.getParameter("size").abs_val * bd.getParameter("shudder").abs_val;
    ofTranslate(ofRandomf() * sh, ofRandomf() * sh, 0);
    float w = bd.getParameter("wobble").abs_val * 180;
    ofRotateZ(ofRandomf() * w);


    p.setFilled(true);
    p.setColor(c);
    p.draw();

    p.setFilled(false);
    p.setStrokeColor(0);
    p.draw();

    ofPopMatrix();

}


void nodeRenderer::fanPointer(ofPtr<clamourNode> n, baseData &bd){

    if(bd.getParameter("flickerOn").abs_val > 0.01 && ofRandomf() < bd.getParameter("flicker").abs_val)return;

    ofPath p = n->getEdgeTemplate();
    ofRectangle r = clamourUtils::getBounds(p);
    ofVec2f v = r.getCenter();

    ofPushMatrix();

    ofScale(screenData::height, screenData::height,1.0);
    ofTranslate(n->getMeanPos_abs());
    if(n->getIsRotate())ofRotate(n->getAvRot());

    int numFans = bd.getParameter("numFans").abs_val;

    float flap = sin(ofDegToRad(ofGetFrameNum()* 5.0/bd.getParameter("flapSpeed").abs_val)) * bd.getParameter("flapMul").abs_val + bd.getParameter("flapAdd").abs_val;
    float floop = sin(ofDegToRad(ofGetFrameNum() * 5.0/bd.getParameter("floopSpeed").abs_val)) * (r.height * bd.getParameter("floopMul").abs_val) + (r.height * bd.getParameter("floopAdd").abs_val);

    float flutter = sin(ofDegToRad(ofGetFrameNum() * 5.0/bd.getParameter("flutterSpeed").abs_val)) * bd.getParameter("flutterMul").abs_val + bd.getParameter("flutterAdd").abs_val;
    float flutterAng = bd.getParameter("flutterAng").abs_val;

    ofColor c1;
    c1.setHsb(bd.getParameter("col1H").abs_val * 255, bd.getParameter("col1S").abs_val * 255, bd.getParameter("col1B").abs_val * 255);

    ofColor c2;
    c2.setHsb(bd.getParameter("col2H").abs_val * 255, bd.getParameter("col2S").abs_val * 255, bd.getParameter("col2B").abs_val * 255);

    for(int i = numFans; i > -1; i --){

        for(int j = 0; j < 2; j ++){

            ofPushMatrix();
            ofTranslate(0,r.height);

            ofRotate(0.5/(float)numFans * (1-j*2) * i * bd.getParameter("fanAngle").abs_val * flap);
            ofTranslate(0, - r.height);

            ofPushMatrix();
            ofTranslate(0, i * floop);

            ofRotate(ofRandom(-flutterAng,flutterAng) * flutter);
            (i%2 ==  0)? p.setColor(c1) : p.setColor(c2);
            p.setFilled(true);
            p.draw();

            p.setFilled(false);
            p.setStrokeColor(0);
            p.draw();

            ofPopMatrix();

            ofPopMatrix();
        }

    }


    p.setColor(c1);
    p.setFilled(true);
    p.draw();

    p.setFilled(false);
    p.setStrokeColor(0);
    p.draw();



    ofPopMatrix();

}
