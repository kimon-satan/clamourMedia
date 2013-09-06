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

class clamourNode{

public:
    
    clamourNode(){};
    clamourNode(int ts, string tr);
    
    void updateHistory();
    
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
    void clearHistory();
    
private:
    
    int smoothFrames;
    int seat;
    string row;
    ofVec2f position;
    ofVec2f meanPos;
    bool isOn;
    vector<ofVec2f> history;
    
    

};


#endif /* defined(__clamourMedia__clamourNode__) */
