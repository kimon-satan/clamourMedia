//
//  paramater.h
//  clamourMedia
//
//  Created by Simon Katan on 12/09/2013.
//
//

#ifndef __clamourMedia__paramater__
#define __clamourMedia__paramater__

#include"ofMain.h"
#include "clamourUtils.h"



class parameter{

public:

    parameter(){};
    parameter(string n, float mv, float mx, float ab, mapType mt, string w= "lin");

    void init(ofVec2f pos);

    float min_val, max_val, abs_val;
    mapType map_type;
    string name;
    string warp;
    string slave;
    int index;

};


class baseData{

public:

    baseData();

    parameter getParameter(string name);


    void init(ofVec2f pos); //sets random and initial variables

    void setParameter(parameter p);
    void update(ofVec2f pos);

    vector<string> getSlaves();
    string getName();
    void setName(string s);
    void setShapeType(string s);
    string getShapeType();
    void setSoundFile(string s);
    string getSoundFile();

    vector<float> getAbsVals();

    virtual ~baseData(){};




private:

    string name;
    map<string, parameter> parameters;
    float warp(string type, float mnv, float mxv, float v);
    string shapeType;
    vector<string> slaveParameters;
    string soundFile;


};



#endif /* defined(__clamourMedia__paramater__) */
