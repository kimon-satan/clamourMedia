#ifndef SPLASHRENDERER_H
#define SPLASHRENDERER_H

#include "title.h"
#include "ofMain.h"
#include "clamourUtils.h"

class splashRenderer{

    public:

    splashRenderer();
    void drawTitles(vector<ofPtr<title> >);

    private:

    ofTrueTypeFont largeTitle;


};

#endif // SPLASHRENDERER_H
