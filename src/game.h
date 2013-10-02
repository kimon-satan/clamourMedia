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
        isSchedulable = true;
        isRepeatable = true;
        schedType = "none";
        interval = 0;
        execAt = 0;
        execNum = 0;
        totExecs = 0;
        interval_secs = 0;

    };

    vector<string> targets;
    vector<string> zTargets;
    vector<selector> selectors;

    int stage, priority;
    int interval, execAt, execNum, totExecs;
    float interval_secs;

    string schedType;

    bool isSchedulable, isRepeatable;

    string mCommand;
    map <string , float> floatParams;
    map <string , int> intParams;
    map <string, string> stringParams;

    zone mZone;
    clamourNode mNode;

    baseData mBaseData; //can be used to store soundDef or DrawDef

    vector<parameter> params; //for changing individual parameters

    vector<reaction> reactions;

    zoneRule onRule, offRule;



};


class game{

    public:

    game();


    void addCommand(command c);
    void reset();
    void incrementStage();
    void decrementStage();


    //getters and setters
    void setName(string s);
    string getName();
    int getCurrentStage();
    int getNumStages();
    int getFurthestStage();

    vector<command> getStageCommands();
    vector<command> getSchedCommands();
    void addSchedCommand(command c);


    private:

    void calcNumStages();

    string name;
    int currentStage, numStages, furthestStage;
    vector<command> mCommands;
    vector<command> mSchedCommands;

};

#endif /* defined(__clamourMedia__game__) */
