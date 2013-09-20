#include "zoneRenderer.h"

zoneRenderer::zoneRenderer()
{
    //ctor
}


void zoneRenderer::draw(ofPtr<zone> z){


    ofVec2f p = z->getPos() * screenHeight;
    float r = z->getRadius() * screenHeight;

    ofNoFill();
    ofSetColor(255);
    ofCircle(p,r);


}


//getters and setters

void zoneRenderer::setScreenDims(int width, int height)
{
    screenWidth = width;
    screenHeight = height;

}



zoneRenderer::~zoneRenderer()
{
    //dtor
}
