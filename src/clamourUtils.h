#ifndef CLAMOURUTILS_H
#define CLAMOURUTILS_H

#define NUM_ROWS 10
#define NUM_SEATS 13

#include "ofMain.h"

enum changeType{

    CLAMOUR_NONE,
    CLAMOUR_ON_OFF,
    CLAMOUR_POSITION,
    CLAMOUR_SOUND,
    CLAMOUR_COUNT

};


enum envType{

    CLAMOUR_ASR,
    CLAMOUR_AR

};

class screenData{

public:

    static int width;
    static int height;

};


class clamourUtils{

    public:

    static int rowToInt(string r);
    static bool pointInPath(ofPath p, ofVec2f v);
    static ofPoint getInsideIntersect(ofPath p, ofPoint centroid, ofPoint invader);

};




#endif // CLAMOURUTILS_H
