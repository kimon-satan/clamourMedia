#ifndef ZONE_H
#define ZONE_H


#include "clamourNode.h"
#include "baseZode.h"

struct zoneRule{

    string ruleType;

    int gtOccupants;
    int ltOccupants;

    vector<string> incDrawTypes;
    vector<string> excDrawTypes;

};

struct zoneEffect{

    //these relate to aspects which effect other nodes, zones, stages etc;

    string effectType;

    //closeOutZone, closeInZone


};


class zone : public baseZode
{
public:
    zone();
    virtual ~zone();

    void addNode(ofPtr<clamourNode> p);
    void removeNode(ofPtr<clamourNode> p);

    //getters and setters

    map<string, ofPtr<clamourNode> > getCaptureNodes();

    void setShape_abs(ofVec2f p, float r); //a circle
    void setShape_abs(vector<ofVec2f> pps); //a poly
    void setShape_abs(ofRectangle r); //duh

    void setShape_rel(ofVec2f p, float r); //a circle
    void setShape_rel(vector<ofVec2f> pps); //a poly
    void setShape_rel(ofRectangle r); //duh

    string getShapeType();

    ofVec2f getPos_abs();
    float getRadius();

    vector<ofVec2f> getPoly_abs();
    ofRectangle getRect_abs();


    bool getIsHidden();
    void setIsHidden(bool b);

    void setIsClosed(bool b);
    bool getIsClosed();

    void setIsLocked(bool b);
    bool getIsLocked();

    void setIsOccupied(bool b);
    bool getIsOccupied();

    void setOnRule(zoneRule zr);
    zoneRule getOnRule();

    void setOffRule(zoneRule zr);
    zoneRule getOffRule();


private:

    bool isOccupied;

    //draw stuff
    bool isHidden;

    //collision stuff
    bool isClosed, isLocked;
    ofVec2f pos_abs;
    ofVec2f pos_rel;
    float radius; //necessarily absolute

    string shapeType;
    vector <ofVec2f> polyPoints_abs;
    vector <ofVec2f> polyPoints_rel;
    ofRectangle rect_abs;
    ofRectangle rect_rel;

    //mechanics stuff
    map <string , ofPtr<clamourNode> > mCaptureNodes;

    zoneRule mOnRule;
    zoneRule mOffRule; // only for ASR zones





};

#endif // ZONE_H
