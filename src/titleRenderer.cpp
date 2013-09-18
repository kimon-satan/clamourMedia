//
//  titleRenderer.cpp
//  clamourMedia
//
//  Created by Simon Katan on 13/09/2013.
//
//

#include "titleRenderer.h"


titleRenderer::titleRenderer(){

    largeTitle.loadFont("fonts/Goulong/Goulong.ttf", 50);




}

void titleRenderer::update(){

    map<string, title>::iterator it;
    //increment the fade ins and outs

    for(it = mTitles.begin(); it != mTitles.end(); it ++){

        if(!it->second.isFadeOut){
            if(it->second.alpha < 255){
                it->second.alpha += (float)(255.0/60.0)/it->second.att_secs;
            }
        }else{
            if(it->second.alpha > 1){
                it->second.alpha -= (float)(255/60)/it->second.dec_secs;
            }else{
                it->second.isEnded = true;
            }
        }
    }

    //remove the spent titles

    for(; it != mTitles.end(); ) {
        if (it->second.isEnded) {
            mTitles.erase(it++);
        } else {
            ++it;
        }
    }

}


void titleRenderer::draw(){



    map<string, title>::iterator it;
    //increment the fade ins and outs

    for(it = mTitles.begin(); it != mTitles.end(); it ++){

        ofSetColor(255, it->second.alpha);
        ofRectangle r(largeTitle.getStringBoundingBox(it->second.text, 0, 0));
        largeTitle.drawString(it->second.text, width/2 - r.width/2, height/2 - r.height/2);

    }


}

//getters and setters

void titleRenderer::setScreen(int w, int h){

    width = w;
    height = h;

}


void titleRenderer::addTitle(string s, title t){

    mTitles[s] = t;
}


void titleRenderer::endTitle(string s){

    mTitles[s].isFadeOut = true;

}
