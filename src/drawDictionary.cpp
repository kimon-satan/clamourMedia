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
    data.setName(dt);


    if(dt == "FLICKER")drawDictionary::flickerDrawData(data);
    if(dt == "ROUND")drawDictionary::roundDrawData(data);

    if(dt == "debugZone")drawDictionary::simpleZone(data);
    if(dt == "pauseButton"){drawDictionary::simpleZone(data);}
    if(dt == "stopButton"){drawDictionary::simpleZone(data);}
    if(dt == "playButton"){drawDictionary::simpleZone(data);}
    if(dt == "revButton"){drawDictionary::simpleZone(data);}
    if(dt == "ffwdButton"){drawDictionary::simpleZone(data);}
    if(dt == "rrwdButton"){drawDictionary::simpleZone(data);}
    if(dt == "ejectButton"){drawDictionary::simpleZone(data);}

    if(dt == "simplePointer")drawDictionary::simplePointerData(data);


    return data;

}

void drawDictionary::simpleZone(baseData &bd){

    bd.setShapeType("roundedRect");
    bd.setParameter(parameter("size", 0, 1, 0.25, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("outerRot", 0, 90, 0, CLAMOUR_MAP_FIXED));

}

void drawDictionary::flickerDrawData(baseData &bd)
{

    bd.setParameter(parameter("flicker", 0.1, 0.75, 0.5, CLAMOUR_MAP_X));
    bd.setParameter(parameter("size", 3, 20, 5, CLAMOUR_MAP_FIXED));

}

void drawDictionary::roundDrawData(baseData &bd)
{
    bd.setParameter(parameter("size", 3, 20, 5, CLAMOUR_MAP_FIXED)); //perhaps size should go as a main parameter ?
}


void drawDictionary::simplePointerData(baseData &bd){

    bd.setName("simplePointer");
    bd.setShapeType("mousePointer"); //may make more sense as separate parameter

    bd.setParameter(parameter("size", 0, 1, 0.06, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("colH", 0, 1, 1, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("colS", 0, 1, 0, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("bFire", 0, 1, 1, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("bOff", 0, 1, 1, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("pulse", 0, 1, -1, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("flicker", 0, 1, -1, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("shudder", 0, 1.0, 0, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("wobble", 0, 1.0, 0, CLAMOUR_MAP_FIXED));

}
