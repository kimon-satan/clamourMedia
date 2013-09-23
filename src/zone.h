#ifndef ZONE_H
#define ZONE_H


#include "clamourNode.h"
#include "baseZode.h"


class zone : public baseZode
{
public:
    zone();
    virtual ~zone();

    void update();
    void react();

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

    void setIsOccupied(bool b);
    bool getIsOccupied();

    bool getIsReacting();
    float getReactProp();

private:

    bool isOccupied;

    //draw stuff
    bool isHidden;

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




};

#endif // ZONE_H
