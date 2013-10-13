#include "zoneRenderer.h"

zoneRenderer::zoneRenderer() {

    //ctor


}

void zoneRenderer::setupPostProc() {

    mPost = ofPtr<ofxPostProcessing>(new ofxPostProcessing());
    mPost->init(screenData::width, screenData::height);
    mPost->createPass<BloomPass>()->setEnabled(true);
    //mPost->createPass<GodRaysPass>()->setEnabled(true);
    mPost->setFlip(false);


}

void zoneRenderer::draw(map<string, ofPtr<zone> > z) {

    // if(!mPost[0]->getEnabled())mPost[0]->setEnabled(true);
    isShaderRender = true;
    map<string, ofPtr<zone> >::iterator it = z.begin();

    ofSetColor(255);

    mPost->begin();

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

    mPost->end();

    /*isShaderRender = false;
    //now for the normal render
    ofPushMatrix();

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

    ofPopMatrix(); */


}

void zoneRenderer::drawDebug(ofPtr<zone> z, baseData &bd) {

    if(isShaderRender)return;

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

    if(isShaderRender){
        if(bd.getParameter("bloom").abs_val < 0.01)return;
    }else{
        if(bd.getParameter("bloom").abs_val > 0)return;
    }

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

    int op =  bd.getParameter("outerPulse").abs_val;
    bool drawOuter = true;
    if(op > 0)drawOuter = (ofGetFrameNum()%op == 0);
    if(drawOuter)drawOuter = ofRandom(0,1) > bd.getParameter("outerFlicker").abs_val;

    float outerOsc = sin(ofDegToRad(ofGetFrameNum() * 5.0/bd.getParameter("outerBCycle").abs_val));
    float outerBm = outerOsc * bd.getParameter("outerBMul").abs_val + bd.getParameter("outerBAdd").abs_val;

    if(drawOuter){

        ofColor outerCol;

        outerCol.setHsb(bd.getParameter("outerH").abs_val * 255,
                        bd.getParameter("outerS").abs_val * 255,
                        bd.getParameter("outerB").abs_val * 255 * outerBm
                        );

        e.setColor(outerCol);

        ofPushMatrix();
            ofRotate(bd.getParameter("outerRot").abs_val);
            e.draw();
            p.draw();
        ofPopMatrix();

    }

    int ip =  bd.getParameter("innerPulse").abs_val;
    bool drawInner = true;
    if(ip > 0)drawInner = (ofGetFrameNum()%ip == 0);
    if(drawInner)drawInner = ofRandom(0,1) > bd.getParameter("innerFlicker").abs_val;

    float innerOsc = sin(ofDegToRad(ofGetFrameNum() * 5.0/bd.getParameter("innerBCycle").abs_val));
    float innerBm = innerOsc * bd.getParameter("innerBMul").abs_val + bd.getParameter("innerBAdd").abs_val;

    if(drawInner){

        ofColor innerCol;

        ofPushMatrix();
        ofRotate(bd.getParameter("innerRot").abs_val);

        innerCol.setHsb(bd.getParameter("innerH").abs_val * 255,
                        bd.getParameter("innerS").abs_val * 255,
                        bd.getParameter("innerB").abs_val * 255 * innerBm
                        );

        ofSetColor(innerCol);
        //draw the symbol
        if(bd.getName() == "pauseButton")pauseSymbol(s, bd.getParameter("innerWarp").abs_val);
        if(bd.getName() == "stopButton")stopSymbol(s);
        if(bd.getName() == "playButton")playSymbol(s);
        if(bd.getName() == "revButton")revPlaySymbol(s);
        if(bd.getName() == "ffwdButton")ffwdSymbol(s, bd.getParameter("innerWarp").abs_val);
        if(bd.getName() == "ejectButton")ejectSymbol(s, bd.getParameter("innerWarp").abs_val);

        ofPopMatrix();

    }

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
    ofPushMatrix();
    ofTranslate(0.15 * w * s, 0);
    ofRect(-s * 0.15 ,0,s/7,s/2);
    ofPopMatrix();

    ofPushMatrix();
    ofTranslate(-0.15 * w * s, 0);
    ofRect(s * 0.15 ,0,s/7,s/2);
    ofPopMatrix();

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

    ofPushMatrix();
        ofTranslate(-0.1 * w,0);
        ofTriangle(
            -s * 0.35, - s * 0.2,
            -s * 0.35, s * 0.2,
            0, 0
        );
    ofPopMatrix();

    ofPushMatrix();
        ofTranslate(0.1 * w,0);
        ofTriangle(
            0, - s * 0.2,
            0, s * 0.2,
            s * 0.35, 0
        );
    ofPopMatrix();

}

void zoneRenderer::rrwdSymbol(float s, float w) {

}

void zoneRenderer::ejectSymbol(float s, float w) {

      ofTriangle(
        s * -0.25, 0 * s,
        s * 0.25, 0 * s,
        0, -s * 0.3
        );

    ofPushStyle();
    ofSetRectMode(OF_RECTMODE_CENTER);
        ofRect(0,0.15 * s,s * 0.5, s * 0.12);
    ofPopStyle();

}



//getters and setters




zoneRenderer::~zoneRenderer() {
    //dtor
}
