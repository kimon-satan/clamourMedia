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

enum changeType{

    CLAMOUR_NONE,
    CLAMOUR_ON_OFF,
    CLAMOUR_POSITION,
    CLAMOUR_SOUND,
    CLAMOUR_COUNT

};

class zone;

class clamourNode{

public:

    clamourNode(){};
    clamourNode(int ts, string tr);

    void updateHistory();
    void modifyHistory();

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

    bool getIsDragOn();
    void setIsDragOn(bool b);

    bool getIsActive();
    void setIsActive(bool b);

    changeType getChanged();
    void setChanged(changeType c);

    void setDrawType(string dt);
    string getDrawType();

    std::tr1::shared_ptr<baseData> getDrawData();

    void setSoundData(baseData sd);
    ofPtr<baseData> getSoundData();

    void clearHistory();
    string getName();

    ofPtr<zone> getZonePair();
    void setZonePair(ofPtr<zone> p);
    void resetZonePair();

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
    bool isDragOn, isActive;

    changeType mChanged;
    vector<ofVec2f> history;
    std::tr1::shared_ptr<baseData> drawData;
    ofPtr<baseData> soundData;

    ofPtr<zone> zonePair;



};


#endif /* defined(__clamourMedia__clamourNode__) */
