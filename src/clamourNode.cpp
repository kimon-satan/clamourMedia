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
    canSleep = true;
    isRotate = false;
    isNewShift = false;

};

clamourNode::clamourNode(){

    rawPos_rel.set(0.5,0.5);
    isDragOn = false;
    smoothFrames = 10;
    shiftAmount = 0.2;
    mChanged = CLAMOUR_NONE;
    isSleeping = true;
    canSleep = true;
    isRotate = false;
    isNewShift = false;

};

void clamourNode::init(){

    soundData.init(meanPos_rel);
    drawData.init(meanPos_rel);

}

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

vector<ofVec2f> clamourNode::getHistory(){
    return history;
}

void clamourNode::updateRotHistory(){

    float ang = 0;

    if(history.size() > 2){
        ofVec2f v = history[history.size()-1] - meanPos_rel;
        if(v.length() > 0.001){
            rotHist.push_back(v);
        }else{
            (rotHist.size() > 0)? rotHist.push_back(rotHist[rotHist.size()-1]): rotHist.push_back(ofVec2f(0,-1)) ;
        }

    }else{
       (rotHist.size() > 0)? rotHist.push_back(rotHist[rotHist.size()-1]): rotHist.push_back(ofVec2f(0,-1)) ;
    }

    if(rotHist.size() > 20)rotHist.erase(rotHist.begin());

    ofVec2f av(0,0);

    for(int i =0; i < rotHist.size(); i++){
        av += rotHist[i];
    }

    av /= (int)rotHist.size();

    av.y *= -1;
    avRot = av.angle(ofVec2f(0,1));

}

void clamourNode::updatePath(){

    outerEdge = edgeTemplate;
   if(isRotate)outerEdge.rotate(avRot ,ofVec3f(0,0,1));
    outerEdge.translate(getMeanPos_abs());

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

void clamourNode::reconcileSlaves(){

    vector<string> ds = drawData.getSlaves();

    for(int i = 0; i < ds.size(); i++){

        parameter dp = drawData.getParameter(ds[i]);
        parameter sp = soundData.getParameter(dp.slave);

        // do the mapping here
        dp.abs_val = ofMap(sp.abs_val, sp.min_val, sp.max_val, dp.min_val, dp.max_val);

        // now put it back again
        drawData.setParameter(dp);

    }


    vector<string> sd = soundData.getSlaves();


    for(int i = 0; i < sd.size(); i++){


        parameter sp = soundData.getParameter(sd[i]);
        parameter dp = drawData.getParameter(sp.slave);

        // do the mapping here
        sp.abs_val = ofMap(dp.abs_val, dp.min_val, dp.max_val, sp.min_val, sp.max_val);
        // now put it back again
        soundData.setParameter(sp);

    }

}

void clamourNode::resetShift(float x , float y)
{

    ofVec2f v(ofVec2f(x,y) * shiftAmount);
    shiftStart.set(meanPos_rel - v);
    isDragOn = true;
    isNewShift = false;

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
    if(animOverride > 0)return;
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
    if(isDragOn)isNewShift = true;
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

void clamourNode::setIsRotate(bool b){ isRotate = b;}
bool clamourNode::getIsRotate(){return isRotate;}

bool clamourNode::getIsNewShift(){return isNewShift;}
void clamourNode::setIsNewShift(bool b){isNewShift = b;}

float clamourNode::getAvRot(){return  avRot;}

void clamourNode::setIntersect(ofVec2f i){
    intersect = i;
}

ofVec2f clamourNode::getIntersect(){
    return intersect;
}
