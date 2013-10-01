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


class drawDictionary{


    public:

    static baseData createDrawData(string dt);

    //drawTypes here
    static void debugDrawData(baseData &bd);
    static void flickerDrawData(baseData &bd);
    static void roundDrawData(baseData &bd);
    static void pauseButtonData(baseData &bd);
    static void mousePointerData(baseData &bd);


};


