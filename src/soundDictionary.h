//
//  soundDictionary.h
//  clamourMedia
//
//  Created by Simon Katan on 12/09/2013.
//
//

#ifndef __clamourMedia__soundDictionary__
#define __clamourMedia__soundDictionary__

#include "parameter.h"
#include "ofxXmlSettings.h"

class soundDictionary{

public:

    static void loadSynthDefs();
    static baseData createSoundData(string synthName);
    static map <string, baseData> synthDictionary;

};


#endif /* defined(__clamourMedia__soundDictionary__) */
