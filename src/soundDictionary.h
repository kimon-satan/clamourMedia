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
    
    static void setup();
    static ofPtr<baseData> createSoundData(string synthName);
    
private:
    
    static map <string, baseData> synthDictionary;
    
};


#endif /* defined(__clamourMedia__soundDictionary__) */
