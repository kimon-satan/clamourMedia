#ifndef SPLASHMANAGER_H
#define SPLASHMANAGER_H

#include "title.h"
#include "baseZode.h"
#include "ofMain.h"

class splashManager
{
public:
    splashManager();
    virtual ~splashManager();

    void update();
    void addTitle(string s, title t);
    void endTitle(string s);
    void reset();
    vector<ofPtr<title> > getOnTitles();

    void addSynth(vector<string> sTargets, baseZode s);
    void endSynth(vector<string> sTargets);
    map<string, ofPtr<baseZode> > getSynths();

    void muteAllSynths();
    void stopAllSynths();

protected:
private:

    map<string, ofPtr<title> > mTitles;
    vector<ofPtr<title> > onTitles;
    map<string, ofPtr<baseZode> > mSynths;


};

#endif // SPLASHMANAGER_H
