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

    map< string, ofPtr<baseZode> >::iterator s_it;
    s_it = mSynths.begin();

    while(s_it != mSynths.end()){
        if (!s_it->second->getIsFiring() && s_it->second->getChanged() == CLAMOUR_NONE) {
            map<string, ofPtr<baseZode> >::iterator toErase = s_it;
            ++s_it;
            mSynths.erase(toErase);
        }else{
            ++s_it;
        }
    }


}


void splashManager::addTitle(string s, title t){

    mTitles[s] = ofPtr<title>(new title(t));
}


void splashManager::endTitle(string s){

    mTitles[s]->isFadeOut = true;

}


void splashManager::addSynth(vector<string> s, baseZode bz){
    cout << "add \n";
//init might be needed to call later

    for(int i = 0; i < s.size(); i++){
        ofPtr<baseZode> ts = ofPtr<baseZode>(new baseZode(bz));
        ts->setName(s[i]);
        ts->setIsFired(true);
        ts->setIsFiring(true);
        ts->setChanged(CLAMOUR_ON_OFF);
        mSynths[ts->getName()] = ts;
    }

}

map<string, ofPtr<baseZode> > splashManager:: getSynths(){

    return mSynths;
}

void splashManager::endSynth(vector<string> s){

    for(int i = 0; i < s.size(); i++){
        mSynths[s[i]]->setIsFired(false);
        mSynths[s[i]]->setIsFiring(false);
        mSynths[s[i]]->setChanged(CLAMOUR_ON_OFF);
    }

}

void splashManager::reset(){

    mTitles.clear();
    onTitles.clear();

    map<string , ofPtr<baseZode> >::iterator it;

    for(it = mSynths.begin(); it != mSynths.end(); it++){
        it->second->setIsFired(false);
        it->second->setIsFiring(false);
        it->second->setChanged(CLAMOUR_ON_OFF);
    }



}

vector<ofPtr<title> >splashManager::getOnTitles(){

    return onTitles;

}

splashManager::~splashManager()
{
    //dtor
}
