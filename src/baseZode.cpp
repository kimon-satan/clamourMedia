#include "baseZode.h"

baseZode::baseZode(){

}

baseZode::~baseZode() {
    //dtor
}

void baseZode::setDrawType(string dt) {

    if(dt != "none"){
        drawData = drawDictionary::createDrawData(dt);
    }else{
        drawData.setName(dt);
    }
}

void baseZode::setDrawData(baseData bd) {
    drawData = bd;
}

void baseZode::setSoundType(string st) {

    if(st != "none"){
        soundData = soundDictionary::createSoundData(st);
    }else{
        soundData.setName(st);
    }
}

baseData baseZode::getDrawData() {
    return drawData;
}

baseData baseZode::getSoundData() {
    return soundData;
}

void baseZode::setSoundData(baseData sd) {
    soundData = sd;

}

void baseZode::setSoundParameter(parameter p) {

    // init needed ?
    p.index = soundData.getParameter(p.name).index;
    p.warp = soundData.getParameter(p.name).warp;
    soundData.setParameter(p);

}

void baseZode::setDrawParameter(parameter p) {

    //init needed ?
    drawData.setParameter(p);

}





changeType baseZode::getChanged() {
    return mChanged;
}

void baseZode::setChanged(changeType c) {
    mChanged = c;
}

void baseZode::setName(string n) {
    name = n;
}

string baseZode::getName() {
    return name;
}




void baseZode::setSoundFile(string s) {
    soundData.setSoundFile(s);
}
string baseZode::getSoundFile() {
    return soundData.getSoundFile();
}

void baseZode::setOuterEdge(ofPath p) {
    outerEdge = p;
}

ofPath baseZode::getOuterEdge() {
    return outerEdge;
}

void baseZode::setEdgeTemplate(ofPath p) {
    edgeTemplate = p;
}

ofPath baseZode::getEdgeTemplate() {
    return edgeTemplate;
}


