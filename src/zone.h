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

    string name;
    bool isOccupied;

    //draw stuff
    bool isHidden;
    string drawType;


private:


    //collision stuff
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
