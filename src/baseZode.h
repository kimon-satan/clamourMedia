#ifndef BASEZODE_H
#define BASEZODE_H


#include "ofMain.h"
#include "drawDictionary.h"
#include "soundDictionary.h"
#include "clamourEvent.h"

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
    baseData getDrawData();
    void setDrawParameter(parameter p);

    void setSoundType(string st);
    virtual void setSoundData(baseData sd);
    baseData getSoundData();
    void setSoundParameter(parameter p);




    void setSoundFile(string s);
    string getSoundFile();

    void setOuterEdge(ofPath p);
    ofPath getOuterEdge();

    void setEdgeTemplate(ofPath p);
    ofPath getEdgeTemplate();


protected:

    string name;
    baseData drawData;
    baseData soundData;
    changeType mChanged;


    ofPath outerEdge;
    ofPath edgeTemplate;


private:


};

#endif // BASEZODE_H
