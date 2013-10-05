#ifndef CLAMOURUTILS_H
#define CLAMOURUTILS_H

#define NUM_ROWS 10
#define NUM_SEATS 13

#include "ofMain.h"

enum mapType{

    CLAMOUR_MAP_FIXED,
    CLAMOUR_MAP_X,
    CLAMOUR_MAP_Y,
    CLAMOUR_MAP_RAND,
    CLAMOUR_MAP_SLAVE,
    CLAMOUR_MAP_COUNT

};

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
    static mapType stringToMapType(string s);

};




#endif // CLAMOURUTILS_H
