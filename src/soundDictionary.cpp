//
//  soundDictionary.cpp
//  clamourMedia
//
//  Created by Simon Katan on 12/09/2013.
//
//

#include "soundDictionary.h"



soundDictionary::soundDictionary(){

    //load the XML definitions into synth Dictionary
    
    ofxXmlSettings XML;
    
    if(XML.loadFile("/Users/simonkatan/Documents/SuperCollider/Code/Projects/clamour/clamourSynths.xml")){
    
        if(XML.pushTag("CLAMOUR_SYNTHS")){
            
            int numSynths = XML.getNumTags("SYNTH");
            
            for(int synth = 0; synth < numSynths; synth++){
                
                if(XML.pushTag("SYNTH", synth)){
                
                    baseData b;
                    
                    b.setName(XML.getValue("NAME", "default"));
                    
                    int numParams = XML.getNumTags("PARAM");
                    
                    for(int param = 0; param < numParams; param++){
                        
                        if(XML.pushTag("PARAM", param)){
                        
                            parameter p;
                            
                            p.name = XML.getValue("NAME", "default");
                            p.min_val = XML.getValue("MIN_VAL", 0.0);
                            p.max_val = XML.getValue("MAX_VAL", 1.0);
                            p.abs_val = XML.getValue("ABS_VAL", 0.5);
                            p.warp = XML.getValue("WARP", "lin");
                            p.map_type = CLAMOUR_MAP_FIXED;
                            p.index = param;
                            
                            b.setParameter(p);
                            
                            XML.popTag(); //PARAM
                        }
                    
                    }
   
                    
                    synthDictionary[b.getName()] = b;
                
                    XML.popTag(); //SYNTH
                }
            
            }
            
            XML.popTag(); //CLAMOUR_SYNTHS
        }
        
    }else{
    
        cout << "clamourSynths.xml has not been found /n";
    };
    
    
    
    
}


baseData soundDictionary::createSoundData(string synthName){
    
    return synthDictionary[synthName]; 
    
    
}

