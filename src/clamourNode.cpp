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
    position.set(0,0);
    isOn = false;
    smoothFrames = 10;
    drawType = CLAMOUR_DRAW_DEBUG;
    
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


//getters and setters

int clamourNode::getSeat(){ return seat;}
void clamourNode::setSeat(int i){seat = i;}
string clamourNode::getRow(){return row;}
void clamourNode::setRow(string s){row = s;}
ofVec2f clamourNode::getPosition(){return position;}
void clamourNode::setPosition(ofVec2f v){position = v;}
ofVec2f clamourNode::getMeanPos(){return meanPos;}
void clamourNode::setMeanPos(ofVec2f v){meanPos = v;}
void clamourNode::clearHistory(){history.clear();}
bool clamourNode::getIsOn(){return isOn;}
void clamourNode::setIsOn(bool b){isOn = b;}

void clamourNode::setDrawType(int i){
    
    drawType = nodeDrawType(i);
    //drawData.reset();
    drawData = drawDictionary::createDrawData(drawType);
    //cout << "dd: " << drawData->getName() << endl;
    drawData->init(meanPos);

}
nodeDrawType clamourNode::getDrawType(){return drawType;}

std::tr1::shared_ptr<baseData> clamourNode::getDrawData(){return drawData;}

ofPtr<baseData> clamourNode::getSoundData(){return soundData;}
void clamourNode::setSoundData(baseData sd){
    
    soundData = ofPtr<baseData>(new baseData(sd));
    soundData->init(meanPos);
}