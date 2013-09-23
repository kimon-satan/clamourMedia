#include "zoneRenderer.h"

zoneRenderer::zoneRenderer()
{
    //ctor
}


void zoneRenderer::draw(map<string, ofPtr<zone> > z){

    map<string, ofPtr<zone> >::iterator it = z.begin();
    ofSetColor(100);

    while(it != z.end()){

        ofVec2f p = it->second->getPos_abs() * screenData::height;
        float r = it->second->getRadius() * screenData::height;

        (it->second->getIsOccupied())?ofFill() : ofNoFill();

        ofCircle(p,r);

        ++it;
    }


}


//getters and setters




zoneRenderer::~zoneRenderer()
{
    //dtor
}
