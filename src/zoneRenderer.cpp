#include "zoneRenderer.h"

zoneRenderer::zoneRenderer()
{
    //ctor
}


void zoneRenderer::draw(ofPtr<zone> z){


    ofVec2f p = z->getPos() * screenData::height;
    float r = z->getRadius() * screenData::height;

    ofNoFill();
    ofSetColor(255);
    ofCircle(p,r);


}


//getters and setters




zoneRenderer::~zoneRenderer()
{
    //dtor
}
