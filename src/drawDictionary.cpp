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

    if(dt == "debugZone")drawDictionary::debugZone(data);
    if(dt == "FLICKER")drawDictionary::flickerDrawData(data);
    if(dt == "ROUND")drawDictionary::roundDrawData(data);
    if(dt == "pauseButton")drawDictionary::pauseButtonData(data);
    if(dt == "simplePointer")drawDictionary::simplePointerData(data);


    return data;

}

void drawDictionary::debugZone(baseData &bd){
    bd.setName("debug");
    bd.setShapeType("roundedRect");
}

void drawDictionary::debugNode(baseData &bd){
    bd.setName("debug");
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


void drawDictionary::simplePointerData(baseData &bd){

    bd.setName("simplePointer");
    bd.setShapeType("mousePointer"); //may make more sense as separate parameter

    bd.setParameter(parameter("size", 0, 1, 0.06, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("colH", 0, 1, 1, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("colS", 0, 1, 0, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("bFire", 0, 1, 1, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("bOff", 0, 1, 1, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("pulse", 0, 1, 0.1, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("flicker", 0, 1, -1, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("shudder", 0, 1.0, 0.1, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("wobble", 0, 1.0, 0.05, CLAMOUR_MAP_FIXED));


}
