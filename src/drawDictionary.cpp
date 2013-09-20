//
//  baseData.cpp
//  clamourMedia
//
//  Created by Simon Katan on 10/09/2013.
//
//

#include "drawDictionary.h"




//draw dictionary methods


std::tr1::shared_ptr<baseData> drawDictionary::createDrawData(string dt)
{

    std::tr1::shared_ptr<baseData> data;

    if(dt == "DEBUG")data = std::tr1::shared_ptr<debugDrawData>(new debugDrawData());
    if(dt == "FLICKER")data = std::tr1::shared_ptr<flickerDrawData>(new flickerDrawData());
    if(dt == "ROUND")data = std::tr1::shared_ptr<roundDrawData>(new roundDrawData());


    return data;

}


flickerDrawData::flickerDrawData()
{

    parameters["flicker"] = parameter("flicker", 0.1, 0.75, 0.5, CLAMOUR_MAP_X);
    parameters["size"] = parameter("size", 3, 20, 5, CLAMOUR_MAP_FIXED);

}

roundDrawData::roundDrawData()
{

    parameters["size"] = parameter("size", 3, 20, 5, CLAMOUR_MAP_FIXED);

}
