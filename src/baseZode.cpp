#include "baseZode.h"

baseZode::baseZode()
{
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

}

baseZode::~baseZode()
{
    //dtor
}

void baseZode::update()
{

    //no modification of isFired from here

    if(mEnvType == CLAMOUR_ASR)
    {

        if(isFired)
        {

            postVal = 0;

            if(attCount > 0)
            {
                attCount -= 1;
                envVal = 1 - (float)attCount/(float)totalAtt;
            }
            else
            {
                envVal =1;
            }

        }
        else
        {

            if(decCount > 0)
            {
                if(postCount > 0)postVal = 1;
                decCount -= 1;
                envVal = (float)decCount/(float)totalDec;

            }
            else
            {

                envVal = 0;

                if(postCount > 0 )
                {
                    postCount -= 1;
                    postVal = (float)postCount/(float)totalPost;
                }
                else
                {
                    postVal = 0;
                    isFiring = false; //NB isFiring ends at the end of the postCount
                }

            }



        }

    }
    else if(mEnvType == CLAMOUR_AR)
    {

        if(isFiring)
        {

            if(attCount > 0)
            {
                attCount -= 1;
                envVal = 1 - (float)attCount/(float)totalAtt;

            }
            else if(decCount > 0)
            {

                if(postCount > 0)postVal = 1;
                decCount -= 1;
                envVal = (float)decCount/(float)totalDec;

            }
            else
            {

                envVal = 0;

                if(postCount > 0 )
                {
                    postCount -= 1;
                    postVal = (float)postCount/(float)totalPost;

                }
                else
                {
                    postVal = 0;
                    isFiring = false;
                }

            }

        }
    }
}

void baseZode::react()
{

    //no modification of isFired from here !
    isFiring = true;

    attCount = attSecs.abs_val * ofGetFrameRate();
    decCount = decSecs.abs_val * ofGetFrameRate();
    postCount = postSecs.abs_val * ofGetFrameRate();
    totalPost = postCount;
    totalDec = decCount;
    totalAtt = attCount;



}

void baseZode::setDrawType(string dt)
{
    drawData = drawDictionary::createDrawData(dt);
}

void baseZode::setDrawData(baseData bd){
    drawData = bd;
}

void baseZode::setSoundType(string st){
    soundData = soundDictionary::createSoundData(st);
}

baseData baseZode::getDrawData()
{
    return drawData;
}

baseData baseZode::getSoundData()
{
    return soundData;
}

void baseZode::setSoundData(baseData sd)
{
    soundData = sd;

}

void baseZode::setSoundParameter(parameter p){

    // init needed ?
    soundData.setParameter(p);

}

void baseZode::setDrawParameter(parameter p){

    //init needed ?
    drawData.setParameter(p);

}

//does the zode have input to fire
bool baseZode::getIsFired()
{
    return isFired;
}
void baseZode::setIsFired(bool b)
{
    isFired = b;
}

//is the zode actually firing
bool baseZode::getIsFiring()
{
    return isFiring;
}

void baseZode::setIsFiring(bool b)
{
    isFiring = b;
}

changeType baseZode::getChanged()
{
    return mChanged;
}

void baseZode::setChanged(changeType c)
{
    mChanged = c;
}

void baseZode::setName(string n)
{
    name = n;
}

string baseZode::getName()
{
    return name;
}


float baseZode::getEnvVal()
{
    return envVal;
}

float baseZode::getPostVal()
{
    return postVal;
}

void baseZode::setAttSecs(float f){

    attSecs.abs_val = f;
    attSecs.map_type = CLAMOUR_MAP_FIXED;

}

void baseZode::setDecSecs(float f){

    decSecs.abs_val = f;
    decSecs.map_type = CLAMOUR_MAP_FIXED;

}


void baseZode::setAttSecs(parameter p){

    attSecs = p;

}

void baseZode::setDecSecs(parameter p){

    decSecs = p;

}

float baseZode::getAttSecs(){return attSecs.abs_val;}
float baseZode::getDecSecs(){return decSecs.abs_val;}

envType baseZode::getEnvType(){return mEnvType;}
void baseZode::setEnvType(string s){
    mEnvType = (s == "ASR")?CLAMOUR_ASR : CLAMOUR_AR;
}
void baseZode::setEnvType(envType t){

    mEnvType = t;

}
