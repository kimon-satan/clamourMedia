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
    name = tr + "_" + ofToString(ts,0);
    position.set(0.5,0.5);
    isOn = false;
    isDragOn = false;
    smoothFrames = 10;
    drawType = "DEBUG";
    shiftAmount = 0.2;
    setCtrlIndex(5);
    isReturnToOn = false;


};

void clamourNode::updateHistory(){

    if(history.size() > smoothFrames){
        history.erase(history.begin());
    }

    history.push_back(position);

    ofVec2f p(0,0);

    for(int i = 0; i < history.size(); i ++){
        p = p + history[i];
    }

    p /= (float)history.size();

    meanPos.set(p);

}

void clamourNode::updateDrawData(){

    drawData->update(meanPos);

}

void clamourNode::updateSoundData(){

    soundData->update(meanPos);

}

void clamourNode::resetShift(float x , float y){


    ofVec2f v(ofVec2f(x,y) * shiftAmount);
    shiftStart.set(meanPos - v);
    isDragOn = true;

}


//getters and setters

int clamourNode::getSeat(){ return seat;}
void clamourNode::setSeat(int i){seat = i;name = row + "_" + ofToString(seat,0);}
string clamourNode::getRow(){return row;}
void clamourNode::setRow(string s){row = s;name = row + "_" + ofToString(seat,0);}
ofVec2f clamourNode::getPosition(){return position;}
void clamourNode::setPosition(ofVec2f v){position = v;}
ofVec2f clamourNode::getMeanPos(){return meanPos;}
void clamourNode::setMeanPos(ofVec2f v){meanPos = v;}
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
    drawData->init(meanPos);

}
string clamourNode::getDrawType(){return drawType;}

std::tr1::shared_ptr<baseData> clamourNode::getDrawData(){return drawData;}

ofPtr<baseData> clamourNode::getSoundData(){return soundData;}
void clamourNode::setSoundData(baseData sd){

    soundData = ofPtr<baseData>(new baseData(sd));
    soundData->init(meanPos);
}

void clamourNode::setCtrlIndex(int len){

    string s = "";

    static const char alphanum[] =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";

    for(int i = 0; i < len; i++){

        s = s + alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    ctrlIndex = s;

}

string clamourNode::getCtrlIndex(){

    return ctrlIndex;

}

bool clamourNode::getIsReturnToOn(){
    return isReturnToOn;
}
void clamourNode::setIsReturnToOn(bool b){
    isReturnToOn = b;
}

string clamourNode::getName(){return name;}
