#include "zone.h"

zone::zone()
{
//ctor
    shapeType = "circle";
    radius = 0.1;
    pos.set(0.5,0.5);

}


//getters and setters
void zone::addNode(string n){
    mCaptureNodes.push_back(n);
}

void zone::removeNode(string n){

    vector<string>::iterator it = remove(mCaptureNodes.begin(), mCaptureNodes.end() ,n);
    if(it != mCaptureNodes.end()){
         mCaptureNodes.erase(it);
    }

}

vector<string> zone::getCaptureNodes(){
    return mCaptureNodes;
}

void zone::setShape(ofVec2f p, float r)
{
    pos = p;
    radius = r;
    shapeType = "circle";
}

void zone::setShape(vector<ofVec2f> pps)
{

    polyPoints = pps;
    shapeType = "poly";

}

void zone::setShape(ofRectangle r)
{

    rect = r;
    shapeType = "rect";

}

string zone::getShapeType()
{
    return shapeType;
}

ofVec2f zone::getPos()
{
    return pos;
}
float zone::getRadius()
{
    return radius;
}
vector<ofVec2f>zone::getPoly()
{
    return polyPoints;
}
ofRectangle zone::getRect()
{
    return rect;
}

void zone::setName(string n){name = n;}
string zone::getName(){return name;}

zone::~zone()
{
//dtor
}
