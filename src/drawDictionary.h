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


class debugDrawData: public baseData{

    public:

    private:
    //internal variables

};

class flickerDrawData: public baseData{

    public:

    flickerDrawData();
    virtual ~flickerDrawData(){};

    private:
    //internal variables


};

class roundDrawData: public baseData{

    public:

    roundDrawData();
    virtual ~roundDrawData(){};

    private:
    //internal variables


};

class drawDictionary{


    public:

    static std::tr1::shared_ptr<baseData> createDrawData(string dt);


};


