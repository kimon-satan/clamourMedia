#ifndef ZONE_H
#define ZONE_H


#include "ofMain.h"
#include "clamourUtils.h"

class zone
{
public:
    zone();
    virtual ~zone();

    //getters and setters
    void setName(string name);
    string getName();

    void addNode(string n);
    void removeNode(string n);
    vector<string> getCaptureNodes();

    void setShape(ofVec2f p, float r); //a circle
    void setShape(vector<ofVec2f> pps); //a poly
    void setShape(ofRectangle r); //duh

    string getShapeType();

    ofVec2f getPos();
    float getRadius();
    vector<ofVec2f> getPoly();
    ofRectangle getRect();

private:

    string name;

    //draw stuff
    bool isHidden;
    string drawType;

    //collision stuff
    ofVec2f pos;
    float radius;

    string shapeType;
    vector <ofVec2f> polyPoints;
    ofRectangle rect;

    //mechanics stuff
    vector <string> mCaptureNodes;


};

#endif // ZONE_H
