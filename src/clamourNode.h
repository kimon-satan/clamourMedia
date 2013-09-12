//
//  clamourNode.h
//  clamourMedia
//
//  Created by Simon Katan on 06/09/2013.
//
//

#ifndef __clamourMedia__clamourNode__
#define __clamourMedia__clamourNode__

#include "ofMain.h"
#include "drawDictionary.h"
#include "soundDictionary.h"



class clamourNode{

public:
    
    clamourNode(){};
    clamourNode(int ts, string tr);
    
    void updateHistory();
    void updateDrawData();
    void updateSoundData();
    
    //getters and setters
    int getSeat();
    void setSeat(int i);
    string getRow();
    void setRow(string s);
    ofVec2f getPosition();
    void setPosition(ofVec2f v);
    ofVec2f getMeanPos();
    void setMeanPos(ofVec2f v);
    bool getIsOn();
    void setIsOn(bool b);
    void setDrawType(int i);
    nodeDrawType getDrawType();
    std::tr1::shared_ptr<baseData> getDrawData();
    
    void setSoundType(string synth);
    ofPtr<baseData> getSoundData();
    
    void clearHistory();
    
private:
    
    int smoothFrames;
    int seat;
    nodeDrawType drawType;
    string row;
    ofVec2f position;
    ofVec2f meanPos;
    bool isOn;
    vector<ofVec2f> history;
    std::tr1::shared_ptr<baseData> drawData;
    ofPtr<baseData> soundData;
    
    

};


#endif /* defined(__clamourMedia__clamourNode__) */
