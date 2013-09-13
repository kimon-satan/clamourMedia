//
//  titleRenderer.h
//  clamourMedia
//
//  Created by Simon Katan on 13/09/2013.
//
//

#ifndef __clamourMedia__titleRenderer__
#define __clamourMedia__titleRenderer__

#include "ofMain.h"

struct title{

    title(){
        
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

class titleRenderer{

    
    public:
    
    titleRenderer();
    
    void update();
    void draw();
    
    void setScreen(int w, int h);
    void addTitle(string s, title t);
    void endTitle(string s);
    
    private:
    
    map<string, title> mTitles;
    
    ofTrueTypeFont largeTitle;
    int width, height;
    


};

#endif /* defined(__clamourMedia__titleRenderer__) */
