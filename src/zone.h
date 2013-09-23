#ifndef ZONE_H
#define ZONE_H


#include "ofMain.h"
#include "clamourUtils.h"
#include "clamourNode.h"



class zone
{
public:
    zone();
    virtual ~zone();

    void update();
    void react();

    void addNode(ofPtr<clamourNode> p);
    void removeNode(ofPtr<clamourNode> p);

    //getters and setters
    void setName(string name);
    string getName();

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

    void setIsOccupied(bool b);
    bool getIsOccupied();

    bool getIsReacting();
    float getReactProp();

    void setChanged(changeType c);
    changeType getChanged();

private:

    string name;
    bool isOccupied;
    changeType mChangeType;

    //draw stuff
    bool isHidden;
    string drawType;
    int reactTotal;
    int reactCount;
    float reactSecs;

    //collision stuff
    bool isClosed;
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

    //sound stuff

    //zone and node need to come from a base class



};

#endif // ZONE_H
