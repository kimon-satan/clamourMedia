#ifndef TITLE_H
#define TITLE_H

#include "ofMain.h"

class title
{

public:
    title()
    {

        alpha = 0;
        isFadeOut = false;
        isEnded = false;
        att_secs = 2.0;
        dec_secs = 1.5;
    }

    string text;
    float alpha;
    bool isFadeOut;
    bool isEnded;
    float att_secs, dec_secs;

};
#endif // TITLE_H
