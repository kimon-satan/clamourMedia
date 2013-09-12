//
//  baseFensterListener.h
//  clamourMedia
//
//  Created by Simon Katan on 06/09/2013.
//
//

#ifndef __clamourMedia__baseFensterListener__
#define __clamourMedia__baseFensterListener__

#include "ofxFenster.h"
#include "nodeManager.h"

class baseFensterListener: public ofxFensterListener {
    
    public:
    
    virtual void setup() = 0;
    virtual void draw() = 0;
    

    
    
};

#endif /* defined(__clamourMedia__baseFensterListener__) */
