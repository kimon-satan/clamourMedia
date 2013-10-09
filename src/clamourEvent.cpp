#include "clamourEvent.h"


clamourEvent::clamourEvent() {

    isFired = false;
    isFiring = false;
    totalAtt = 0;
    totalDec = 0;
    attCount = 0;
    decCount = 0;
    postCount =0;
    envVal = 0;
    postVal = 0;

    attSecs.abs_val = 0.1; //need to be xml methods for this
    decSecs.abs_val = 2;
    postSecs.abs_val = 0;

    mEnvType = CLAMOUR_ASR;
    animOverride = 0;

}


void clamourEvent::update() {

    //no modification of isFired from here

    if(mEnvType == CLAMOUR_ASR) {

        if(isFired) {

            postVal = 0;

            if(attCount > 0) {
                attCount -= 1;
                envVal = 1 - (float)attCount/(float)totalAtt;
            } else {
                envVal =1;
            }

        } else {

            if(decCount > 0) {
                if(postCount > 0)postVal = 1;
                decCount -= 1;
                envVal = (float)decCount/(float)totalDec;

            } else {

                envVal = 0;

                if(postCount > 0 ) {
                    postCount -= 1;
                    postVal = (float)postCount/(float)totalPost;
                } else {
                    postVal = 0;
                    isFiring = false; //NB isFiring ends at the end of the postCount
                }

            }



        }

    } else if(mEnvType == CLAMOUR_AR) {

        if(isFiring) {

            if(attCount > 0) {
                attCount -= 1;
                envVal = 1 - (float)attCount/(float)totalAtt;

            } else if(decCount > 0) {

                if(postCount > 0)postVal = 1;
                decCount -= 1;
                envVal = (float)decCount/(float)totalDec;

            } else {

                envVal = 0;

                if(postCount > 0 ) {
                    postCount -= 1;
                    postVal = (float)postCount/(float)totalPost;

                } else {
                    postVal = 0;
                    isFiring = false;
                }

            }

        }
    }

    if(animOverride > 0){
        animOverride -= 1;
    }
}

void clamourEvent::react() {

    //no modification of isFired from here !
    isFiring = true;

    attCount = attSecs.abs_val * ofGetFrameRate();
    decCount = decSecs.abs_val * ofGetFrameRate();
    postCount = postSecs.abs_val * ofGetFrameRate();
    totalPost = postCount;
    totalDec = decCount;
    totalAtt = attCount;



}

//does the zode have input to fire
bool clamourEvent::getIsFired() {
    return isFired;
}
void clamourEvent::setIsFired(bool b) {
    isFired = b;
}

//is the zode actually firing
bool clamourEvent::getIsFiring() {
    return isFiring;
}

void clamourEvent::setIsFiring(bool b) {
    isFiring = b;
}


float clamourEvent::getEnvVal() {
    return envVal;
}

float clamourEvent::getPostVal() {
    return postVal;
}

void clamourEvent::setAttSecs(float f) {

    attSecs.abs_val = f;
    attSecs.map_type = CLAMOUR_MAP_FIXED;

}

void clamourEvent::setDecSecs(float f) {

    decSecs.abs_val = f;
    decSecs.map_type = CLAMOUR_MAP_FIXED;

}


void clamourEvent::setAttSecs(parameter p) {

    attSecs = p;

}

void clamourEvent::setDecSecs(parameter p) {
    decSecs = p;
}

float clamourEvent::getAttSecs() {
    return attSecs.abs_val;
}
float clamourEvent::getDecSecs() {
    return decSecs.abs_val;
}

envType clamourEvent::getEnvType() {
    return mEnvType;
}
void clamourEvent::setEnvType(string s) {
    mEnvType = (s == "ASR")?CLAMOUR_ASR : CLAMOUR_AR;
}

void clamourEvent::setEnvType(envType t) {
    mEnvType = t;
}

//this belongs in clamourNode !
void clamourEvent::setAnimOverride(int i){
    animOverride = i;
}

int clamourEvent::getAnimOverride(){
    return animOverride;
}

int clamourEvent::getExecAt(){
    return execAt;
}

void clamourEvent::setExecAt(int i){execAt = i;}

clamourEvent::~clamourEvent()
{
    //dtor
}
