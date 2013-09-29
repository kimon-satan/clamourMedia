//
//  game.h
//  clamourMedia
//
//  Created by Simon Katan on 10/09/2013.
//
//

#ifndef __clamourMedia__game__
#define __clamourMedia__game__

#include "ofMain.h"
#include "clientManager.h" //for selectors
#include "clamourNode.h"
#include "zone.h"

struct command{

    command(){

        priority = 0;
        stage = 0;
        mCommand = "";
    };

    vector<string> targets;
    vector<string> zTargets;
    vector<selector> selectors;
    int stage;
    int priority;

    string mCommand;
    map <string , float> floatParams;
    map <string , int> intParams;
    map <string, string> stringParams;

    zone mZone;
    clamourNode mNode;

    baseData mBaseData; //can be used to store soundDef or DrawDef

    vector<parameter> params; //for changing individual parameters


};


class game{

    public:

    game();


    void addCommand(command c);
    void reset();
    void incrementStage();


    //getters and setters
    void setName(string s);
    string getName();
    int getCurrentStage();
    int getNumStages();

    vector<command> getStageCommands();


    private:

    void calcNumStages();

    string name;
    int currentStage, numStages;
    vector<command> mCommands;

};

#endif /* defined(__clamourMedia__game__) */
