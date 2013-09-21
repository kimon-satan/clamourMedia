#ifndef SPLASHMANAGER_H
#define SPLASHMANAGER_H

#include "title.h"
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

protected:
private:

    map<string, ofPtr<title> > mTitles;
    vector<ofPtr<title> > onTitles;


};

#endif // SPLASHMANAGER_H
