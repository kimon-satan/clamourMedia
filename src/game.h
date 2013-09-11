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

struct group{

    string name;
    vector<string> indexes;
    
};

struct command{

    command(){
        
        priority = 0;
        stage = 0;
        mCommand = "";
    };
    
    vector<string> targets;
    int stage;
    int priority;
    
    string mCommand;
    map <string , float> floatParams;
    map <string , int> intParams;
    map <string, string> stringParams;
    

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
