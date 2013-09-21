#include "splashManager.h"

splashManager::splashManager()
{
    //ctor
}

void splashManager::update(){

    map<string, ofPtr<title> >::iterator it;
    //increment the fade ins and outs

    for(it = mTitles.begin(); it != mTitles.end(); it ++){

        if(!it->second->isFadeOut){
            if(it->second->alpha < 255){
                it->second->alpha += (float)(255.0/60.0)/it->second->att_secs;
            }
        }else{
            if(it->second->alpha > 1){
                it->second->alpha -= (float)(255/60)/it->second->dec_secs;
            }else{
                it->second->isEnded = true;
            }
        }
    }

    //remove the spent titles
    onTitles.clear();
    it = mTitles.begin();

    while(it != mTitles.end()){
        if (it->second->isEnded) {
            map<string, ofPtr<title> >::iterator toErase = it;
            ++it;
            mTitles.erase(toErase);
        } else {
            onTitles.push_back(it->second);
            ++it;
        }
    }

}


void splashManager::addTitle(string s, title t){

    mTitles[s] = ofPtr<title>(new title(t));
}


void splashManager::endTitle(string s){

    mTitles[s]->isFadeOut = true;

}


void splashManager::reset(){

    mTitles.clear();
    onTitles.clear();

}

vector<ofPtr<title> >splashManager::getOnTitles(){

    return onTitles;

}

splashManager::~splashManager()
{
    //dtor
}
