//
//  baseData.cpp
//  clamourMedia
//
//  Created by Simon Katan on 10/09/2013.
//
//

#include "drawDictionary.h"




//draw dictionary methods


baseData drawDictionary::createDrawData(string dt) {

    baseData data;
    data.setName(dt);


    if(dt == "FLICKER")drawDictionary::flickerDrawData(data);
    if(dt == "ROUND")drawDictionary::roundDrawData(data);

    if(dt == "debugZone")drawDictionary::simpleZone(data);
    if(dt == "pauseButton")drawDictionary::simpleZone(data);
    if(dt == "stopButton") {
        drawDictionary::simpleZone(data);
    }
    if(dt == "playButton") {
        drawDictionary::simpleZone(data);
    }
    if(dt == "revButton") {
        drawDictionary::simpleZone(data);
    }
    if(dt == "ffwdButton") {
        drawDictionary::simpleZone(data);
    }
    if(dt == "rrwdButton") {
        drawDictionary::simpleZone(data);
    }
    if(dt == "ejectButton") {
        drawDictionary::simpleZone(data);
    }
    if(dt == "jogButton") {
        drawDictionary::simpleZone(data);
    }


    if(dt == "simplePointer")drawDictionary::simplePointerData(data);
    if(dt == "fanPointer")drawDictionary::fanPointerData(data);

    return data;

}

void drawDictionary::simpleZone(baseData &bd) {

    bd.setShapeType("roundedRect");
    bd.setParameter(parameter("size", 0, 1, 0.25, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("outerRot", 0, 90, 0, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("innerRot", 0, 90, 0, CLAMOUR_MAP_FIXED));

    bd.setParameter(parameter("bloom", 0, 1, 0.0, CLAMOUR_MAP_FIXED));

    bd.setParameter(parameter("innerB", 0, 1, 1.0, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("outerB", 0, 1, 1.0, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("innerH", 0, 1, 0, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("outerH", 0, 1, 0.5, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("innerS", 0, 1, 1.0, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("outerS", 0, 1, 0, CLAMOUR_MAP_FIXED));

    bd.setParameter(parameter("innerFlicker", 0, 1, 0, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("innerPulse", 0, 5, 0, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("outerFlicker", 0, 1, 0, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("outerPulse", 0, 5, 0, CLAMOUR_MAP_FIXED));

    bd.setParameter(parameter("outerBAdd", 0, 1, 1,CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("outerBMul", 0, 1,0,CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("outerBCycle", 0, 1,2.0,CLAMOUR_MAP_FIXED));

    bd.setParameter(parameter("innerBAdd", 0, 1, 1,CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("innerBMul", 0, 1, 0,CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("innerBCycle", 0, 1,2.0,CLAMOUR_MAP_FIXED));

    bd.setParameter(parameter("innerWarp", 0, 1, 0, CLAMOUR_MAP_FIXED));

}

void drawDictionary::flickerDrawData(baseData &bd) {

    bd.setParameter(parameter("flicker", 0.1, 0.75, 0.5, CLAMOUR_MAP_X));
    bd.setParameter(parameter("size", 3, 20, 5, CLAMOUR_MAP_FIXED));

}

void drawDictionary::roundDrawData(baseData &bd) {
    bd.setParameter(parameter("size", 3, 20, 5, CLAMOUR_MAP_FIXED)); //perhaps size should go as a main parameter ?
}


void drawDictionary::simplePointerData(baseData &bd) {

    bd.setName("simplePointer");
    bd.setShapeType("mousePointer"); //may make more sense as separate parameter

    bd.setParameter(parameter("size", 0, 1, 0.06, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("colH", 0, 1, 1, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("colS", 0, 1, 0, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("bFire", 0, 1, 1, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("bOff", 0, 1, 1, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("pulse", 0, 1, -1, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("pulseOn", 0, 1, -1, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("flicker", 0, 1, -1, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("flickerOn", 0, 1, -1, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("shudder", 0, 1.0, 0, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("wobble", 0, 1.0, 0, CLAMOUR_MAP_FIXED));

}

void drawDictionary::fanPointerData(baseData &bd) {

    bd.setName("fanPointer");
    bd.setShapeType("mousePointerStraight"); //may make more sense as separate parameter

    bd.setParameter(parameter("size", 0, 1, 0.06, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("col1H", 0, 1, 1, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("col1S", 0, 1, 0, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("col1B", 0, 1, 1, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("col2H", 0, 1, 1, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("col2S", 0, 1, 0, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("col2B", 0, 1, 1, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("flicker", 0, 1, -1, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("flickerOn", 0, 1, -1, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("fanAngle", 0, 360, 80, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("numFans", 1, 20, 10, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("floopMul", 0, 1, 0, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("floopAdd", 0, 1, 0, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("floopSpeed", 1, 10, 5, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("flapSpeed", 1, 10, 2, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("flapMul", 0, 1, 0, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("flapAdd", 0, 1, 1, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("flutterAng", 0, 10, 0, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("flutterSpeed", 1, 10, 0, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("flutterAdd", 0, 1, 0.5, CLAMOUR_MAP_FIXED));
    bd.setParameter(parameter("flutterMul", 0, 1, 0.5, CLAMOUR_MAP_FIXED));

}


