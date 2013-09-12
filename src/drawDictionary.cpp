//
//  baseData.cpp
//  clamourMedia
//
//  Created by Simon Katan on 10/09/2013.
//
//

#include "drawDictionary.h"




//draw dictionary methods


std::tr1::shared_ptr<baseData> drawDictionary::createDrawData(nodeDrawType d){

    std::tr1::shared_ptr<baseData> data;

    switch(d){
        case CLAMOUR_DRAW_DEBUG:data = std::tr1::shared_ptr<debugDrawData>(new debugDrawData());break;
        case CLAMOUR_DRAW_FLICKER:data = std::tr1::shared_ptr<flickerDrawData>(new flickerDrawData());break;
    }
    
    return data;
    
}


flickerDrawData::flickerDrawData(){

    parameters["flicker"] = parameter("flicker", 0.1, 0.75, 0.5, CLAMOUR_MAP_X);
    parameters["size"] = parameter("size", 3, 20, 5, CLAMOUR_MAP_RAND);
    
}
