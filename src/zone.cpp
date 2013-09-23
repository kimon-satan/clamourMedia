#include "zone.h"

zone::zone()
{
//ctor
    setShape_rel(ofVec2f(0.5,0.5),0.15);
    isHidden = false;
    isOccupied = false;
    isClosed = false;
}


void zone::addNode(ofPtr<clamourNode> n){
    mCaptureNodes[n->getName()] = n;
    isOccupied = true;
}

void zone::removeNode(ofPtr<clamourNode> n){
    mCaptureNodes.erase(n->getName());
    if(mCaptureNodes.size() == 0)isOccupied = false;
}


//getters and setters

void zone::setName(string n){name = n;}
string zone::getName(){return name;}

map<string, ofPtr<clamourNode> > zone::getCaptureNodes(){
    return mCaptureNodes;
}

void zone::setShape_abs(ofVec2f p, float r)
{
    pos_abs = p;
    pos_rel = ofVec2f(p.x * (float)screenData::height/screenData::width, p.y);
    radius = r;
    shapeType = "circle";
}

void zone::setShape_abs(vector<ofVec2f> pps)
{
    polyPoints_abs = pps;
    //todo: go through and convert points to relative
    shapeType = "poly";
}

void zone::setShape_abs(ofRectangle r)
{
    rect_abs = r;
     //todo: go through and convert dims to relative
    shapeType = "rect";
}

void zone::setShape_rel(ofVec2f p, float r){
    pos_rel = p;
    pos_abs = ofVec2f(p.x * (float)screenData::width/screenData::height, p.y);
    radius = r;
    shapeType = "circle";
}

void zone::setShape_rel(vector<ofVec2f> pps){

}

void zone::setShape_rel(ofRectangle r){

}


string zone::getShapeType()
{
    return shapeType;
}

ofVec2f zone::getPos_abs()
{
    return pos_abs;
}

float zone::getRadius()
{
    return radius;
}

vector<ofVec2f>zone::getPoly_abs()
{
    return polyPoints_abs;
}

ofRectangle zone::getRect_abs()
{
    return rect_abs;
}

void zone::setIsOccupied(bool b){isOccupied =b;}
bool zone::getIsOccupied(){return isOccupied;}

bool zone::getIsHidden(){ return isHidden; }
void zone::setIsHidden(bool b){isHidden = b; }

void zone::setIsClosed(bool b){ isClosed = b;}
bool zone::getIsClosed(){ return isClosed;}

zone::~zone()
{
//dtor
}
