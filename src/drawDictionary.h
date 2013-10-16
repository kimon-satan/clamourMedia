//
//  baseData.h
//  clamourMedia
//
//  Created by Simon Katan on 10/09/2013.
//
//

#pragma once

#include "ofMain.h"
#include "parameter.h"


class zoneData : public baseData{


    public:

    //internal variables
    bool isBloomOn;
    int bloomCycleCount;

};

class drawDictionary{


    public:

    static baseData createDrawData(string dt);
    static zoneData createZoneDrawData();

    //drawTypes here
    static void simpleZone(baseData &bd);
    static void debugNode(baseData &bd);
    static void flickerDrawData(baseData &bd);
    static void roundDrawData(baseData &bd);
    static void simplePointerData(baseData &bd);
    static void fanPointerData(baseData &bd);




};




