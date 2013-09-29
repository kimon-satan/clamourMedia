#ifndef BASEZODE_H
#define BASEZODE_H

#include "clamourUtils.h"
#include "ofMain.h"
#include "drawDictionary.h"
#include "soundDictionary.h"

class baseZode
{

public:
    baseZode();
    virtual ~baseZode();

    void update();
    void react();

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

    //does the zode have input to fire
    bool getIsFired();
    void setIsFired(bool b);

    //is the zode actually firing
    bool getIsFiring();
    void setIsFiring(bool b);

    float getEnvVal();
    float getPostVal();

    float getAttSecs();
    float getDecSecs();

    void setAttSecs(float f);
    void setDecSecs(float f);

    void setAttSecs(parameter p);
    void setDecSecs(parameter p);

    envType getEnvType();
    void setEnvType(string s);
    void setEnvType(envType t);

protected:

    string name;
    baseData drawData;
    baseData soundData;
    changeType mChanged;

    bool isFired, isFiring;

    envType mEnvType;

    int totalAtt, totalDec, totalPost, attCount, decCount, postCount;
	float envVal, postVal;
    parameter attSecs, decSecs, postSecs;


private:


};

#endif // BASEZODE_H
