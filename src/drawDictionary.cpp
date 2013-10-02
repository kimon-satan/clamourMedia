//
//  baseData.cpp
//  clamourMedia
//
//  Created by Simon Katan on 10/09/2013.
//
//

#include "drawDictionary.h"




//draw dictionary methods


baseData drawDictionary::createDrawData(string dt)
{

    baseData data;

    if(dt == "DEBUG")drawDictionary::debugDrawData(data);
    if(dt == "FLICKER")drawDictionary::flickerDrawData(data);
    if(dt == "ROUND")drawDictionary::roundDrawData(data);
    if(dt == "pauseButton")drawDictionary::pauseButtonData(data);
    if(dt == "mousePointer")drawDictionary::mousePointerData(data);

    return data;

}


void drawDictionary::debugDrawData(baseData &bd){
    bd.setName("DEBUG");
}

void drawDictionary::flickerDrawData(baseData &bd)
{

    bd.setName("FLICKER");
    bd.setParameter(parameter("flicker", 0.1, 0.75, 0.5, CLAMOUR_MAP_X));
    bd.setParameter(parameter("size", 3, 20, 5, CLAMOUR_MAP_FIXED));

}

void drawDictionary::roundDrawData(baseData &bd)
{

    bd.setName("ROUND");
    bd.setParameter(parameter("size", 3, 20, 5, CLAMOUR_MAP_FIXED)); //perhaps size should go as a main parameter ?

}

void drawDictionary::pauseButtonData(baseData &bd)
{

    bd.setName("pauseButton");
    bd.setShapeType("roundedRect");

}

void drawDictionary::mousePointerData(baseData &bd){

    bd.setName("mousePointer");
    bd.setShapeType("mousePointer");
    bd.setParameter(parameter("flicker", 0, 1, 0, CLAMOUR_MAP_FIXED));

}
