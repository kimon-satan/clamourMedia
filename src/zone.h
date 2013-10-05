#ifndef ZONE_H
#define ZONE_H


#include "clamourNode.h"
#include "baseZode.h"

struct zoneRule{

    zoneRule(){
        isEnabled = true;
    };

    string ruleType;
    bool isEnabled;

    int gtOccupants;
    int ltOccupants;

    vector<string> incDrawTypes;
    vector<string> excDrawTypes;

};

struct reaction{

    //these relate to aspects which effect other nodes, zones, stages etc;
    string rType;

    //closeOutZone, openOutZone, closeInZone, openInZone, incrementStage
    string trig; //ON, OFF, ON_OFF

    map<string, string> stringParams;
    map<string, float> floatParams;

    vector<clamourNode> prevNodeStates; // easier just to store the whole node


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

    void setPos_rel(ofVec2f p);
    void setPos_abs(ofVec2f p);

    void setShape_abs(ofVec2f p, float r); //a circle
    void setShape_rel(ofVec2f p, float r); //a circle

    void recalcAbsDims();

    string getShapeType();

    ofVec2f getPos_abs();
    float getRadius();

    vector<ofVec2f> getPoly_abs();
    ofRectangle getRect_abs();

    void setIsClosedOut(bool b);
    bool getIsClosedOut();

    void setIsClosedIn(bool b);
    bool getIsClosedIn();

    void setOnRule(zoneRule zr);
    zoneRule getOnRule();

    void setOffRule(zoneRule zr);
    zoneRule getOffRule();

    vector<reaction> getReactions();
    void setReactions(vector<reaction>);
    void addReaction(reaction e);


private:


    //collision stuff
    bool isClosedOut, isClosedIn;
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
    zoneRule mOffRule;

    vector<reaction > mReactions;


};

#endif // ZONE_H
