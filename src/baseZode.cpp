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

baseData & baseZode::getDrawData() {
    return drawData;
}

baseData & baseZode::getSoundData() {
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

vector<reaction >baseZode::getReactions()
{
    return mReactions;
}

void baseZode::addReaction(reaction e){

    mReactions.push_back(e);

}

void baseZode::setReactions(vector<reaction> r){

    mReactions = r;

}

void baseZode::triggerEvent(int i){

    if(mEvents.size() > i -1){

        mEvents[i - 1].react();
        mEvents[i - 1].setIsFired(true);
        for(int s = 0; s < mSounds.size(); s ++){
            if(mSounds[s].getEventIndex() == i){
                scMessenger::startSynth(name, mEvents[i - 1], mSounds[s]);
            }
            //send the osc to superCollider via OSC manager
        }
    }
}

void baseZode::addEvent(clamourEvent e){

    mEvents.push_back(e);

}

void baseZode::updateEvents(){

    for(int i = 0; i < mEvents.size(); i ++){
        mEvents[i].update();
    }
}


void baseZode::endEvent(int i){

    if(mEvents.size() > i -1){

         mEvents[i - 1].setIsFired(false);

        for(int s = 0; s < mSounds.size(); s ++){
            if(mSounds[s].getEventIndex() == i){
                scMessenger::stopSynth(name, mSounds[s]);
            }
            //send the osc to superCollider via OSC manager
        }
    }

}

void baseZode::endSound(){

     scMessenger::stopSynth(name, soundData);
    for(int s = 0; s < mSounds.size(); s ++){

        scMessenger::stopSynth(name, mSounds[s]);

    }

}

void baseZode::endEvents(){

    for(int i = 0; i < mEvents.size(); i ++)endEvent(i + 1);
}

//getters and setters
void baseZode::addSound(baseData bd){

    mSounds.push_back(bd);
}

void baseZode::setEvents(vector<clamourEvent> e){
    mEvents = e;
}

vector<clamourEvent> baseZode::getEvents(){
    return mEvents;
}

vector<baseData> baseZode::getSounds(){
    return mSounds;
}

void baseZode::setSounds(vector<baseData> b){
    mSounds = b;
}

