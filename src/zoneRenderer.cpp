#include "zoneRenderer.h"

zoneRenderer::zoneRenderer()
{
    //ctor
}


void zoneRenderer::draw(map<string, ofPtr<zone> > z){

    map<string, ofPtr<zone> >::iterator it = z.begin();

     for(map<string, ofPtr<zone> >::iterator it = z.begin(); it != z.end(); it ++) {

        baseData bd = it->second->getDrawData();

        if(bd.getName() == "debug")drawDebug(it->second, bd);
        if(bd.getName() == "pauseButton")pauseButton(it->second, bd);

    }

}

void zoneRenderer::drawDebug(ofPtr<zone> z, baseData &bd){

    ofPath p = z->getOuterEdge();

    ofPushMatrix();

    ofScale(screenData::height, screenData::height,1.0);

    if(z->getIsFiring()){
        p.setFilled(true);
        p.setColor(z->getEnvVal() * 255.0);
        p.draw();
    }
        p.setFilled(false);
        p.setStrokeColor(255);
        p.draw();

    ofPopMatrix();

}

void zoneRenderer::pauseButton(ofPtr<zone> z, baseData &bd){

    ofPath p = z->getEdgeTemplate();
    ofPath e = p;
    p.setFilled(true);
    e.setFilled(true);
    e.scale(0.9,0.9);
    e.setColor(0);


    ofPushMatrix();


    ofScale(screenData::height, screenData::height,1.0);
    ofTranslate(z->getPos_abs().x, z->getPos_abs().y, 0);

    float col = max(z->getEnvVal() * 255.0,100.0);

        p.setColor(col);

        p.draw();
        e.draw();

        ofPushStyle();
            ofSetRectMode(OF_RECTMODE_CENTER);
            ofSetColor(col);
            ofRect(0,0,0.05,0.05); //HACKED do from size variable !!!
        ofPopStyle();

    ofPopMatrix();

}


//getters and setters




zoneRenderer::~zoneRenderer()
{
    //dtor
}
