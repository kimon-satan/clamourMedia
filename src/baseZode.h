#ifndef BASEZODE_H
#define BASEZODE_H


#include "ofMain.h"
#include "drawDictionary.h"
#include "soundDictionary.h"
#include "clamourEvent.h"
#include "scMessenger.h"

class clamourNode;

struct reaction {

    //these relate to aspects which effect other nodes, zones, stages etc;
    string rType;

    map<string, string> stringParams;
    map<string, float> floatParams;
    map<string, int> intParams;


    //only used for zones
  //  vector<clamourNode> prevNodeStates; // easier just to store the whole node
      //closeOutZone, openOutZone, closeInZone, openInZone, incrementStage
    string trig; //ON, OFF, ON_OFF
    vector<string> zTargets;
    ofPtr<clamourNode> tempNode;

};


struct eventComm{

    string ownerIndex;
    reaction r;
    int execAt;
    int eventIndex;

};


class baseZode : public clamourEvent {

public:
    baseZode();
    virtual ~baseZode();

    //getters and setters
    void setName(string name);
    string getName();

    changeType getChanged();
    void setChanged(changeType c);

    void setDrawData(baseData bd);
    virtual void setDrawType(string dt);
    baseData &getDrawData();
    void setDrawParameter(parameter p);

    void setSoundType(string st);
    virtual void setSoundData(baseData sd);
    baseData &getSoundData();
    void setSoundParameter(parameter p);


    void setSoundFile(string s);
    string getSoundFile();

    void setOuterEdge(ofPath p);
    ofPath getOuterEdge();

    void setEdgeTemplate(ofPath p);
    ofPath getEdgeTemplate();

    vector<reaction> getReactions();
    void setReactions(vector<reaction>);
    void addReaction(reaction e);

    void addSound(baseData bd);
    void endSound();


    void triggerEvent(int i);
    void endEvent(int i);
    void endEvents();

    void addEvent(clamourEvent e);
    void updateEvents();

    vector<clamourEvent> getEvents();
    void setEvents(vector<clamourEvent> e);

    vector<baseData> getSounds();
    void setSounds(vector<baseData> bd);

    void silenceSounds();

protected:

    string name;
    baseData drawData;
    baseData soundData;
    changeType mChanged;


    ofPath outerEdge;
    ofPath edgeTemplate;

    vector<reaction > mReactions;

    //extra

    vector<clamourEvent> mEvents;
    vector<baseData> mSounds;


private:


};

#endif // BASEZODE_H
