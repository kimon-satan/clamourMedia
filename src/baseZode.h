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

protected:

    string name;
    std::tr1::shared_ptr<baseData> drawData;
    ofPtr<baseData> soundData;
    changeType mChanged;
    string drawType;

private:


};

#endif // BASEZODE_H
