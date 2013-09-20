//
//  clamourNode.cpp
//  clamourMedia
//
//  Created by Simon Katan on 06/09/2013.
//
//

#include "clamourNode.h"


clamourNode::clamourNode(int ts, string tr){

    seat = ts;
    row = tr;
    name = tr + "_" + ofToString(ts);
    rawPos_rel.set(0.5,0.5);
    isOn = false;
    isDragOn = false;
    smoothFrames = 10;
    drawType = "DEBUG";
    shiftAmount = 0.2;
    isReturnToOn = false;


};

void clamourNode::updateHistory(){

    if(history.size() > smoothFrames){
        history.erase(history.begin());
    }

    //constrain to edges
    rawPos_rel.x = max(0.0f, min(1.0f, rawPos_rel.x));
    rawPos_rel.y = max(0.0f, min(1.0f, rawPos_rel.y));

    history.push_back(rawPos_rel);

    ofVec2f p(0,0);

    for(int i = 0; i < history.size(); i ++){
        p = p + history[i];
    }

    p /= (float)history.size();

    meanPos_rel.set(p);

}

void clamourNode::updateDrawData(){

    drawData->update(meanPos_rel); //used for mappings

}

void clamourNode::updateSoundData(){

    soundData->update(meanPos_rel); //used for mappings

}

void clamourNode::resetShift(float x , float y){


    ofVec2f v(ofVec2f(x,y) * shiftAmount);
    shiftStart.set(meanPos_rel - v);
    isDragOn = true;

}


//getters and setters

int clamourNode::getSeat(){ return seat;}
void clamourNode::setSeat(int i){
    seat = i;
    name = row + "_" + ofToString(seat);
    cout << name << endl;
}
string clamourNode::getRow(){return row;}
void clamourNode::setRow(string s){row = s;name = row + "_" + ofToString(seat);}

void clamourNode::setRawPos_rel(ofVec2f v){rawPos_rel = v;}
ofVec2f clamourNode::getMeanPos_rel(){return meanPos_rel;}

void clamourNode::setRawPos_abs(ofVec2f v){

    float f = (float)screenData::height/screenData::width;
    rawPos_rel.set(v.x * f, v.y);

}

ofVec2f clamourNode::getMeanPos_abs(){

    float f = (float)screenData::width/screenData::height;
    return ofVec2f(meanPos_rel.x * f, meanPos_rel.y);

}


ofVec2f clamourNode::getShiftStart(){return shiftStart;}
void clamourNode::setShiftAmount(float f){shiftAmount = f;}
float clamourNode::getShiftAmount(){return shiftAmount;}
void clamourNode::clearHistory(){history.clear();}
bool clamourNode::getIsOn(){return isOn;}
void clamourNode::setIsOn(bool b){isOn = b;}
bool clamourNode::getIsDragOn(){return isDragOn;}
void clamourNode::setIsDragOn(bool b){isDragOn = b;}

void clamourNode::setDrawType(string dt){

    drawType = dt;
    drawData = drawDictionary::createDrawData(drawType);
    drawData->init(meanPos_rel);

}
string clamourNode::getDrawType(){return drawType;}

std::tr1::shared_ptr<baseData> clamourNode::getDrawData(){return drawData;}

ofPtr<baseData> clamourNode::getSoundData(){return soundData;}
void clamourNode::setSoundData(baseData sd){

    soundData = ofPtr<baseData>(new baseData(sd));
    soundData->init(meanPos_rel);
}


bool clamourNode::getIsReturnToOn(){
    return isReturnToOn;
}
void clamourNode::setIsReturnToOn(bool b){
    isReturnToOn = b;
}

string clamourNode::getName(){return name;}
