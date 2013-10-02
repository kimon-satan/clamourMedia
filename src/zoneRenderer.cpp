#include "zoneRenderer.h"

zoneRenderer::zoneRenderer()
{
    //ctor
}


void zoneRenderer::draw(map<string, ofPtr<zone> > z){

    map<string, ofPtr<zone> >::iterator it = z.begin();

    while(it != z.end()){

        ofSetColor(255);

        drawDebug(it->second);

        ++it;
    }


}

void zoneRenderer::drawDebug(ofPtr<zone> z){

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


//getters and setters




zoneRenderer::~zoneRenderer()
{
    //dtor
}
