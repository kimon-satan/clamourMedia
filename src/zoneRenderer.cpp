#include "zoneRenderer.h"

zoneRenderer::zoneRenderer() {
    //ctor
}


void zoneRenderer::draw(map<string, ofPtr<zone> > z) {

    map<string, ofPtr<zone> >::iterator it = z.begin();

    for(map<string, ofPtr<zone> >::iterator it = z.begin(); it != z.end(); it ++) {

        baseData bd = it->second->getDrawData();

        if(bd.getName() == "debugZone")drawDebug(it->second, bd);
        if(bd.getName() == "pauseButton")simpleButton(it->second, bd);
        if(bd.getName() == "stopButton")simpleButton(it->second, bd);
        if(bd.getName() == "playButton")simpleButton(it->second, bd);
        if(bd.getName() == "revButton")simpleButton(it->second, bd);
        if(bd.getName() == "ffwdButton")simpleButton(it->second, bd);
        if(bd.getName() == "rrwdButton")simpleButton(it->second, bd);
        if(bd.getName() == "ejectButton")simpleButton(it->second, bd);

    }

}

void zoneRenderer::drawDebug(ofPtr<zone> z, baseData &bd) {

    ofPath p = z->getOuterEdge();

    ofPushMatrix();

    ofScale(screenData::height, screenData::height,1.0);

    if(z->getIsFiring()) {
        p.setFilled(true);
        p.setColor(z->getEnvVal() * 255.0);
        p.draw();
    }
    p.setFilled(false);
    p.setStrokeColor(255);
    p.draw();

    ofPopMatrix();

}

void zoneRenderer::simpleButton(ofPtr<zone> z, baseData &bd) {

    ofPath p = z->getEdgeTemplate();
    ofPath e = p;
    p.setFilled(true);
    p.setColor(0);
    e.setFilled(true);
    e.scale(1.1,1.1);

    float s = bd.getParameter("size").abs_val;

    ofPushMatrix();

    ofScale(screenData::height, screenData::height,1.0);
    ofTranslate(z->getPos_abs().x, z->getPos_abs().y, 0);

    float col = max(z->getEnvVal() * 255.0,100.0);

    e.setColor(col);

    ofPushMatrix();
        ofRotate(ofGetFrameNum());
        e.draw();
        p.draw();
    ofPopMatrix();

    ofSetColor(col);
    //draw the symbol
    if(bd.getName() == "pauseButton")pauseSymbol(s, 1 - z->getEnvVal());
    if(bd.getName() == "stopButton")stopSymbol(s);
    if(bd.getName() == "playButton")playSymbol(s);
    if(bd.getName() == "revButton")revPlaySymbol(s);
    if(bd.getName() == "ffwdButton")ffwdSymbol(s, 1 - z->getEnvVal());

    ofPopMatrix();

}

void zoneRenderer::stopSymbol(float s) {

    ofPushStyle();
    ofSetRectMode(OF_RECTMODE_CENTER);

    ofRect(0,0,s/5,s/5);
    ofPopStyle();

}

void zoneRenderer::pauseSymbol(float s, float w) {

    ofPushStyle();
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofRect(-s * 0.15 * w,0,s/7,s/2);
    ofRect(s * 0.15 * w,0,s/7,s/2);
    ofPopStyle();

}

void zoneRenderer::playSymbol(float s) {

    ofTriangle(
                -s * 0.25, - s * 0.25,
                -s * 0.25, s * 0.25,
                s * 0.25, 0
                );
}

void zoneRenderer::revPlaySymbol(float s) {

    ofTriangle(
            s * 0.25, - s * 0.25,
            s * 0.25, s * 0.25,
            -s * 0.25, 0
            );

}

void zoneRenderer::ffwdSymbol(float s, float w) {

    ofTriangle(
                -s * 0.35, - s * 0.2,
                -s * 0.35, s * 0.2,
                0, 0
                );

   ofTriangle(
            0, - s * 0.2,
            0, s * 0.2,
            s * 0.35, 0
            );

}

void zoneRenderer::rrwdSymbol(float s, float w) {

}

void zoneRenderer::ejectSymbol(float s, float w) {


}



//getters and setters




zoneRenderer::~zoneRenderer() {
    //dtor
}
