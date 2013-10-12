//
//  paramater.cpp
//  clamourMedia
//
//  Created by Simon Katan on 12/09/2013.
//
//

#include "parameter.h"

baseData::baseData(){
    soundFile = "none";
    name = "none";
    eventIndex = 0;
}

void baseData::init(ofVec2f pos){

    map<string, parameter>::iterator it;

    for(it = parameters.begin(); it != parameters.end(); it++){

        it->second.init(pos);
    }

}


void baseData::update(ofVec2f pos){

    vector<float> f;
    f.push_back(0);
    update(pos, f);

}

void baseData::update(ofVec2f pos, vector<float> & envVals){

    //update all the parameters according to mappings
    //could put a different menthod for zones to use other params such as num occupants but leave for now

    map<string, parameter>::iterator it;

    for(it = parameters.begin(); it != parameters.end(); it++){

        switch(it->second.map_type){

            case CLAMOUR_MAP_X:
                it->second.abs_val = warp(it->second.warp, pos.x, it->second.min_val, it->second.max_val);
                break;
            case CLAMOUR_MAP_Y:
                it->second.abs_val = warp(it->second.warp, 1 - pos.y, it->second.min_val, it->second.max_val);
                break;
            case CLAMOUR_MAP_EVENT:
                it->second.abs_val = warp(it->second.warp, envVals[it->second.envIndex], it->second.min_val, it->second.max_val);
                break;

        }



    }

}

parameter & baseData::getParameter(string name){

    try{

        return parameters[name];
    }

    catch(int e){

        cout << "this parameter doesn't exist" << endl;
    }

}

void baseData::setParameter(parameter p){

    parameters[p.name] = p;
    if(p.map_type == CLAMOUR_MAP_SLAVE){

        if(find(slaveParameters.begin(),slaveParameters.end(), p.name) != slaveParameters.end()){

        }else{
            slaveParameters.push_back(p.name);
        }

    }else{
        vector<string>::iterator it = remove(slaveParameters.begin(), slaveParameters.end(), p.name);
        if(it != slaveParameters.end())slaveParameters.erase(it);
    }

}

vector<string> baseData::getSlaves(){

    return slaveParameters;
}


string baseData::getName(){return name;}

void baseData::setName(string s){name = s;}


float baseData::warp(string type, float v, float mnv, float mxv){



    //potentially add safeties for 0 values or cross pole vals
    if(type == "exp"){
        return pow((mxv/mnv),v) * mnv;
    }else{
        return ofMap(v, 0, 1, mnv, mxv);
    }

}

vector<float> baseData::getAbsVals(){

    vector<float> vals(parameters.size());

    map<string, parameter>::iterator it;

    for(it = parameters.begin(); it != parameters.end(); it++){

        vals[it->second.index] = it->second.abs_val; //ordered by index

    }


    return vals;
}

void baseData::setShapeType(string s){shapeType = s;}
string baseData::getShapeType(){return shapeType;}

void baseData::setSoundFile(string s){soundFile = s;}
string baseData::getSoundFile(){return soundFile;}

void baseData::setEventIndex(int i){eventIndex = i;}
int baseData::getEventIndex(){return eventIndex;}

void baseData::setInternal(string s, float f){
    internalsF[s] = f;
}

void baseData::setInternal(string s, int i){
    internalsI[s] = i;
}

float baseData::getInternalF(string s){
    return internalsF[s];
}

float baseData::getInternalI(string s){
    return internalsI[s];
}

/*-----------------------------------------------------------------------------------*/


parameter::parameter(string n, float mv, float mx, float ab, mapType mt, string w):
    name(n), min_val(mv), max_val(mx), abs_val(ab), map_type(mt), warp(w){


}

void parameter::init(ofVec2f pos){

    switch(map_type){ //later more for intial node positions

        case CLAMOUR_MAP_RAND:
            abs_val = ofRandom(min_val, max_val);
            break;
        case CLAMOUR_MAP_EVENT:
            abs_val = min_val;
            break;

    }


}



