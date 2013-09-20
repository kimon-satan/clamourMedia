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
#include "clamourUtils.h"

class clamourNode{

public:

    clamourNode(){};
    clamourNode(int ts, string tr);

    void updateHistory();
    void updateDrawData();
    void updateSoundData();
    void resetShift(float x, float y);


    //getters and setters
    int getSeat();
    void setSeat(int i);
    string getRow();
    void setRow(string s);

    ofVec2f getMeanPos_rel();
    ofVec2f getMeanPos_abs();

    void setRawPos_abs(ofVec2f v);
    void setRawPos_rel(ofVec2f v);

    ofVec2f getShiftStart();
    float getShiftAmount();
    void setShiftAmount(float f);
    bool getIsOn();
    void setIsOn(bool b);
    bool getIsDragOn();
    void setIsDragOn(bool b);
    bool getIsReturnToOn();
    void setIsReturnToOn(bool b);
    void setDrawType(string dt);
    string getDrawType();
    std::tr1::shared_ptr<baseData> getDrawData();

    void setSoundData(baseData sd);
    ofPtr<baseData> getSoundData();

    void clearHistory();
    string getName();

private:

    string name;
    int smoothFrames;
    int seat;
    string ctrlIndex;
    string drawType;
    string row;

    ofVec2f rawPos_rel;
    ofVec2f meanPos_rel;
    ofVec2f meanPos_abs;

    ofVec2f shiftStart;
    float shiftAmount;
    bool isOn, isDragOn, isReturnToOn;
    vector<ofVec2f> history;
    std::tr1::shared_ptr<baseData> drawData;
    ofPtr<baseData> soundData;



};


#endif /* defined(__clamourMedia__clamourNode__) */
