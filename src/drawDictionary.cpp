//
//  baseDrawData.cpp
//  clamourMedia
//
//  Created by Simon Katan on 10/09/2013.
//
//

#include "drawDictionary.h"


parameter::parameter(string n, float mv, float mx, float ab, mapType mt): name(n), min_val(mv), max_val(mx), abs_val(ab), map_type(mt){

    
    switch(map_type){
            
        case CLAMOUR_MAP_RAND:
            abs_val = ofRandom(min_val, max_val);
            break;
       
            
    }
    
    
}

//base draw data

void baseDrawData::update(ofVec2f pos){
    
    //update all the parameters according to mappings
    
    map<string, parameter>::iterator it;
    
    for(it = parameters.begin(); it != parameters.end(); it++){
        
        switch(it->second.map_type){
                
            case CLAMOUR_MAP_X:
                it->second.abs_val = ofMap(pos.x, 0, 1, it->second.min_val, it->second.max_val);
                break;
            case CLAMOUR_MAP_Y:
                it->second.abs_val = ofMap(pos.y, 1, 0, it->second.min_val, it->second.max_val);
                break;
    
        }
        
        
    }

}

parameter baseDrawData::getParameter(string name){
    
    try{
        
        return parameters[name];
    }
    
    catch(int e){
    
        cout << "this parameter doesn't exist" << endl;
    }
    
}

void baseDrawData::setParameter(parameter p){

    parameters[p.name] = p;
    
}


//draw dictionary methods


std::tr1::shared_ptr<baseDrawData> drawDictionary::createDrawData(nodeDrawType d){

    std::tr1::shared_ptr<baseDrawData> data;

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
