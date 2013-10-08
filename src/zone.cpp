#include "zone.h"

zone::zone()
{
//ctor

    isClosedOut = false;
    isClosedIn = false;

    mChanged = CLAMOUR_NONE;

    mOnRule.isEnabled = false;
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

void zone::addEvent(clamourEvent e){

    mEvents.push_back(e);

}

void zone::updateEvents(){

    for(int i = 0; i < mEvents.size(); i ++){
        mEvents[i].update();
    }
}


void zone::updateDrawData()
{
    vector<float> f;
    f.push_back(envVal);

    for(int i = 0; i < mEvents.size(); i ++){
        f.push_back(mEvents[i].getEnvVal());
    }

    drawData.update(pos_rel, f); //used for mappings & events

}

void zone::triggerEvent(int i){

    mEvents[i - 1].react();
}

//getters and setters



map<string, ofPtr<clamourNode> > zone::getCaptureNodes()
{
    return mCaptureNodes;
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


void zone::recalcAbsDims()
{
    //ultimately will depend on shapeType
    pos_abs = ofVec2f(pos_rel.x * (float)screenData::width/screenData::height, pos_rel.y);
    outerEdge = edgeTemplate;
    outerEdge.translate(ofPoint(pos_abs.x, pos_abs.y));

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

vector<reaction >zone::getReactions()
{
    return mReactions;
}

void zone::addReaction(reaction e){

    mReactions.push_back(e);

}

void zone::setReactions(vector<reaction> r){

    mReactions = r;

}

void zone::setPos_abs(ofVec2f v)
{

    pos_abs = v;
    float f = (float)screenData::height/(float)screenData::width;
    pos_rel.set(v.x * f, v.y);

}

void zone::setPos_rel(ofVec2f v)
{

    pos_rel = v;
    float f = (float)screenData::width/(float)screenData::height;
    pos_abs.set(v.x * f, v.y);

}

zone::~zone()
{
//dtor
}
