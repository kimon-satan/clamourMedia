#ifndef CLAMOUREVENT_H
#define CLAMOUREVENT_H

#include "clamourUtils.h"
#include "parameter.h"

class clamourEvent {

public:
    clamourEvent();
    virtual ~clamourEvent();

    void update();
    void react();

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

    void setAnimOverride(int i);
    int getAnimOverride();

   //does the zode have input to fire
    bool getIsFired();
    void setIsFired(bool b);

    //is the zode actually firing
    bool getIsFiring();
    void setIsFiring(bool b);

protected:

    int animOverride; // really should be in ClamourNode
    bool isFiring;
    bool isFired;

    envType mEnvType;

    int totalAtt, totalDec, totalPost, attCount, decCount, postCount;
    float envVal, postVal;
    parameter attSecs, decSecs, postSecs;

private:

};

#endif // CLAMOUREVENT_H
