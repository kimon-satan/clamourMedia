//
//  clamourNode.h
//  clamourMedia
//
//  Created by Simon Katan on 06/09/2013.
//
//

#ifndef __clamourMedia__clamourNode__
#define __clamourMedia__clamourNode__


#include "baseZode.h"


class zone;

class clamourNode : public baseZode {

public:

    clamourNode();
    clamourNode(int ts, string tr);

    void updateHistory();
    void modifyHistory();

    void updatePath();

    void setDrawType(string dt);
    void updateDrawData();
    void setSoundData(baseData sd);
    void updateSoundData();
    void resetShift(float x, float y);
    void reconcileSlaves();

    void init();

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

    void setIsRotate(bool b);
    bool getIsRotate();

    void clearHistory();
    vector<ofVec2f> getHistory();
    void updateRotHistory();

    ofPtr<zone> getZonePair();
    void setZonePair(ofPtr<zone> p);
    void resetZonePair();

    bool getIsSleeping();
    void setIsSleeping(bool b);

    bool getCanSleep();
    void setCanSleep(bool b);

    bool getIsNewShift();
    void setIsNewShift(bool b);

    float getAvRot();

    void setIntersect(ofVec2f i);
    ofVec2f getIntersect();

    ofRectangle getBounds();

    bool getIsColliding();
    void setIsColliding(bool b);

    bool getIsCollidable();
    void setIsCollidable(bool b);

    bool getIsDistributable();
    void setIsDistributable(bool b);

private:

    bool isSleeping, canSleep;

    int smoothFrames;
    int seat;
    string ctrlIndex;

    string row;

    ofVec2f rawPos_rel;
    ofVec2f meanPos_rel;
    ofVec2f meanPos_abs;
    ofVec2f intersect;

    ofVec2f shiftStart;
    float shiftAmount;
    bool isDragOn;
    bool isNewShift;
    float avRot;
    bool isRotate;

    vector<ofVec2f> history;
    vector<ofVec2f> rotHist;
    ofPtr<zone> zonePair;

    ofRectangle bounds;

    bool isColliding , isCollidable;
    bool isDistributable;


};


#endif /* defined(__clamourMedia__clamourNode__) */
