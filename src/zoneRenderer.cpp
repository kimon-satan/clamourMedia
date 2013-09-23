#include "zoneRenderer.h"

zoneRenderer::zoneRenderer()
{
    //ctor
}


void zoneRenderer::draw(map<string, ofPtr<zone> > z){

    map<string, ofPtr<zone> >::iterator it = z.begin();

    while(it != z.end()){

        ofSetColor(255);

        ofVec2f p = it->second->getPos_abs() * screenData::height;
        float r = it->second->getRadius() * screenData::height;

        ofNoFill();
        ofCircle(p,r);

        if(it->second->getIsReacting()){
            ofSetColor(it->second->getReactProp() * 255.0);
            ofFill();
            ofCircle(p,r);
        }

        ++it;
    }


}


//getters and setters




zoneRenderer::~zoneRenderer()
{
    //dtor
}
