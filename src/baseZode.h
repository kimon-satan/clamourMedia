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

    virtual void setDrawType(string dt);
    string getDrawType();

    std::tr1::shared_ptr<baseData> getDrawData();

    virtual void setSoundData(baseData sd);
    ofPtr<baseData> getSoundData();

    //does the zode have input to fire
    bool getIsFired();
    void setIsFired(bool b);

    //is the zode actually firing
    bool getIsFiring();
    void setIsFiring(bool b);

    float getEnvVal();
    float getPostVal();

protected:

    string name;
    std::tr1::shared_ptr<baseData> drawData;
    ofPtr<baseData> soundData;
    changeType mChanged;
    string drawType;

    bool isFired, isFiring;

    envType mEnvType;

    int totalAtt, totalDec, totalPost, attCount, decCount, postCount;
	float envVal, postVal;
    parameter attSecs, decSecs, postSecs;


private:


};

#endif // BASEZODE_H
