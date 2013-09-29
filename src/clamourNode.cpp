//
//  clamourNode.cpp
//  clamourMedia
//
//  Created by Simon Katan on 06/09/2013.
//
//

#include "clamourNode.h"


clamourNode::clamourNode(int ts, string tr)
{

    seat = ts;
    row = tr;
    name = tr + "_" + ofToString(ts);
    rawPos_rel.set(0.5,0.5);
    isDragOn = false;
    smoothFrames = 10;
    shiftAmount = 0.2;
    mChanged = CLAMOUR_NONE;
    isSleeping = true;
    canSleep = false;

};

void clamourNode::updateHistory()
{

    if(history.size() > smoothFrames)
    {
        history.erase(history.begin());
    }

    //constrain to edges
    rawPos_rel.x = max(0.0f, min(1.0f, rawPos_rel.x));
    rawPos_rel.y = max(0.0f, min(1.0f, rawPos_rel.y));

    history.push_back(rawPos_rel);

    ofVec2f p(0,0);

    for(int i = 0; i < history.size(); i ++)
    {
        p = p + history[i];
    }

    p /= (float)history.size();

    meanPos_rel.set(p);



}

void clamourNode::modifyHistory(){

    //for when a new position has been set after the history was updated

    //constrain to edges
    rawPos_rel.x = max(0.0f, min(1.0f, rawPos_rel.x));
    rawPos_rel.y = max(0.0f, min(1.0f, rawPos_rel.y));

    history[history.size() - 1] = rawPos_rel; //replace the last term

    ofVec2f p(0,0);

    for(int i = 0; i < history.size(); i ++)
    {
        p = p + history[i];
    }

    p /= (float)history.size();

    meanPos_rel.set(p);

}

void clamourNode::setDrawType(string dt)
{
    //an override of baseZode
    drawData = drawDictionary::createDrawData(dt);
    drawData.init(meanPos_rel);

}

void clamourNode::updateDrawData()
{

    drawData.update(meanPos_rel); //used for mappings

}

void clamourNode::setSoundData(baseData sd)
{
    soundData = sd;
    soundData.init(meanPos_rel); //override
}

void clamourNode::updateSoundData()
{

    soundData.update(meanPos_rel); //used for mappings

}

void clamourNode::resetShift(float x , float y)
{

    ofVec2f v(ofVec2f(x,y) * shiftAmount);
    shiftStart.set(meanPos_rel - v);
    isDragOn = true;

}


//getters and setters

int clamourNode::getSeat()
{
    return seat;
}
void clamourNode::setSeat(int i)
{
    seat = i;
    name = row + "_" + ofToString(seat);

}
string clamourNode::getRow()
{
    return row;
}
void clamourNode::setRow(string s)
{
    row = s;
    name = row + "_" + ofToString(seat);
}

void clamourNode::setRawPos_rel(ofVec2f v)
{

    rawPos_rel = v;
}

ofVec2f clamourNode::getMeanPos_rel()
{
    return meanPos_rel;
}

void clamourNode::setRawPos_abs(ofVec2f v)
{

    float f = (float)screenData::height/(float)screenData::width;
    rawPos_rel.set(v.x * f, v.y);

}

ofVec2f clamourNode::getMeanPos_abs()
{

    float f = (float)screenData::width/screenData::height;
    return ofVec2f(meanPos_rel.x * f, meanPos_rel.y);

}


ofVec2f clamourNode::getShiftStart()
{
    return shiftStart;
}
void clamourNode::setShiftAmount(float f)
{
    shiftAmount = f;
}
float clamourNode::getShiftAmount()
{
    return shiftAmount;
}
void clamourNode::clearHistory()
{
    history.clear();
}

bool clamourNode::getIsDragOn()
{
    return isDragOn;
}
void clamourNode::setIsDragOn(bool b)
{
    isDragOn = b;
}


ofPtr<zone> clamourNode::getZonePair()
{
    return zonePair;
}

void clamourNode::setZonePair(ofPtr<zone> p)
{
    zonePair = p;
}

void clamourNode::resetZonePair()
{
    zonePair.reset();
}

bool clamourNode::getIsSleeping(){
    return isSleeping;
}

void clamourNode::setIsSleeping(bool b){
    isSleeping = b;
}

bool clamourNode::getCanSleep(){
    return canSleep;
}

void clamourNode::setCanSleep(bool b){
    canSleep = b;
}

