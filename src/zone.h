#ifndef ZONE_H
#define ZONE_H


#include "clamourNode.h"
#include "baseZode.h"
#include "scMessenger.h"

struct zoneRule {

    zoneRule() {
        isEnabled = true;
    };

    string ruleType;
    bool isEnabled;

    int gtOccupants;
    int ltOccupants;

    vector<string> incDrawTypes;
    vector<string> excDrawTypes;

};

struct reaction {

    //these relate to aspects which effect other nodes, zones, stages etc;
    string rType;

    //closeOutZone, openOutZone, closeInZone, openInZone, incrementStage
    string trig; //ON, OFF, ON_OFF
    vector<string> zTargets;

    map<string, string> stringParams;
    map<string, float> floatParams;
    map<string, int> intParams;

    vector<clamourNode> prevNodeStates; // easier just to store the whole node


};


class zone : public baseZode {
public:
    zone();
    virtual ~zone();

    void init();

    void addNode(ofPtr<clamourNode> p);
    void removeNode(ofPtr<clamourNode> p);

    void addEvent(clamourEvent e);

    void updateEvents();
    void updateDrawData();
    //getters and setters

    void triggerEvent(int i);
    void endEvent(int i);
    void endEvents();

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

    void addSound(baseData bd);
    void endSound();

    void setIsBloom(bool b);
    bool getIsBloom();

    void setFlickerCount(int i);
    int getFlickerCount();


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

    //extra

    vector<clamourEvent> mEvents;
    vector<baseData> mSounds;

    //internal draw stuff that has no place being here

    bool isBloom;
    int flickerCount;
};

#endif // ZONE_H
