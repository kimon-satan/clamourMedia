//
//  baseDrawData.h
//  clamourMedia
//
//  Created by Simon Katan on 10/09/2013.
//
//

#pragma once

#include "ofMain.h"


enum nodeDrawType{
    
    CLAMOUR_DRAW_DEBUG,
    CLAMOUR_DRAW_FLICKER,
    CLAMOUR_DRAW_COUNT
    
};

enum mapType{

    CLAMOUR_MAP_FIXED,
    CLAMOUR_MAP_X,
    CLAMOUR_MAP_Y,
    CLAMOUR_MAP_RAND,
    CLAMOUR_MAP_COUNT

};

class parameter{

    public:
    
    parameter(){};
    parameter(string n, float mv, float mx, float ab, mapType mt);
    
    float min_val, max_val, abs_val;
    mapType map_type;
    string name;

};


class baseDrawData{
    
    public:
    
    baseDrawData(){};
    parameter getParameter(string name);
    void setParameter(parameter p);
    void update(ofVec2f pos);
    virtual ~baseDrawData(){};
    
    
    protected:
    
    map<string, parameter> parameters;


};

class debugDrawData: public baseDrawData{
    
    public:
    
    private:
    //internal variables
    
};

class flickerDrawData: public baseDrawData{
    
    public:
    
    flickerDrawData();
    virtual ~flickerDrawData(){};
    
    
    private:
    //internal variables
    
    


};

class drawDictionary{


    public:
    
    static std::tr1::shared_ptr<baseDrawData> createDrawData(nodeDrawType d);
    
    
};


