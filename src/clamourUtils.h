#ifndef CLAMOURUTILS_H
#define CLAMOURUTILS_H

#define NUM_ROWS 10
#define NUM_SEATS 10

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





#endif // CLAMOURUTILS_H
