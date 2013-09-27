#include "zone.h"

zone::zone()
{
//ctor
    setShape_rel(ofVec2f(0.5,0.5),0.15);
    isClosedOut = false;
    isClosedIn = false;

    mChanged = CLAMOUR_NONE;

    mOnRule.ruleType = "MIN_OCCUPANTS";
    mOnRule.gtOccupants = 1;
    mOnRule.isEnabled = true;
    mOffRule.isEnabled = false;

}


void zone::addNode(ofPtr<clamourNode> n)
{
    mCaptureNodes[n->getName()] = n;
}

void zone::removeNode(ofPtr<clamourNode> n)
{
    mCaptureNodes.erase(n->getName());
}


//getters and setters



map<string, ofPtr<clamourNode> > zone::getCaptureNodes()
{
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

void zone::setShape_rel(ofVec2f p, float r)
{
    pos_rel = p;
    pos_abs = ofVec2f(p.x * (float)screenData::width/screenData::height, p.y);
    radius = r;
    shapeType = "circle";
}

void zone::setShape_rel(vector<ofVec2f> pps)
{

}

void zone::setShape_rel(ofRectangle r)
{

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

void zone::recalcAbsDims()
{

    //ultimately will depend on shapeType
    if(shapeType == "circle")
    {
        pos_abs = ofVec2f(pos_rel.x * (float)screenData::width/screenData::height, pos_rel.y);
    }

}

void zone::setOnRule(zoneRule zr)
{
    mOnRule = zr;
}
zoneRule zone::getOnRule()
{
    return mOnRule;
}

void zone::setOffRule(zoneRule zr)
{
    mOffRule = zr;
}
zoneRule zone::getOffRule()
{
    return mOffRule;
}



void zone::setIsClosedOut(bool b)
{
    isClosedOut = b;
}

bool zone::getIsClosedOut()
{
    return isClosedOut;
}

void zone::setIsClosedIn(bool b)
{
    isClosedIn = b;
}

bool zone::getIsClosedIn()
{
    return isClosedIn;
}

vector<ofPtr<zoneEffect> >zone::getReactions()
{
    return reactions;
}

void zone::addReaction(zoneEffect e){

    ofPtr<zoneEffect> ze = ofPtr<zoneEffect>(new zoneEffect(e));
    reactions.push_back(ze);

}

zone::~zone()
{
//dtor
}
