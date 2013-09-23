#include "baseZode.h"

baseZode::baseZode()
{
    //ctor
}

baseZode::~baseZode()
{
    //dtor
}

void baseZode::setDrawType(string dt)
{

    drawType = dt;
    drawData = drawDictionary::createDrawData(drawType);
    //drawData->init(meanPos_rel); //override in node

}

string baseZode::getDrawType()
{
    return drawType;
}

std::tr1::shared_ptr<baseData> baseZode::getDrawData()
{
    return drawData;
}

ofPtr<baseData> baseZode::getSoundData()
{
    return soundData;
}

void baseZode::setSoundData(baseData sd)
{
    soundData = ofPtr<baseData>(new baseData(sd));
   // soundData->init(meanPos_rel); //override
}



changeType baseZode::getChanged(){return mChanged; }
void baseZode::setChanged(changeType c){
    mChanged = c;
}

void baseZode::setName(string n){name = n;}
string baseZode::getName(){return name;}
