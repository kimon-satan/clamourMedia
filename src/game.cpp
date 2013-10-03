//
//  game.cpp
//  clamourMedia
//
//  Created by Simon Katan on 10/09/2013.
//
//

#include "game.h"

bool sortByPriority(command a, command b){return (a.priority < b.priority);}

game::game(){

    numStages = 0;
    reset();

}




void game::addCommand(command c){

    mCommands.push_back(c);
    calcNumStages();

}

void game::reset(){

    mSchedCommands.clear();
    currentStage = 0;
    furthestStage = 0;
    //potentially alot more here depending on mechanics
}

void game::incrementStage(){

    currentStage = min(numStages- 1, currentStage + 1);
    furthestStage = max(currentStage, furthestStage);

}

void game::decrementStage(){

    currentStage = max(0, currentStage - 1);

}

void game::calcNumStages(){

    //update the number of stages for the new command
    for(int i = 0; i < mCommands.size(); i++){

        if(mCommands[i].stage + 1 > numStages)numStages = mCommands[i].stage + 1;

    }

}

void game::setName(string s){name = s;}
string game::getName(){return name;}
int game::getCurrentStage(){ return currentStage;}
int game::getNumStages(){return numStages;}
int game::getFurthestStage(){return furthestStage;}

vector<command> game::getStageCommands(){

    vector<command> c;

    for(int i = 0; i < mCommands.size(); i ++){
        if(mCommands[i].stage == currentStage)c.push_back(mCommands[i]);
    }

    sort(c.begin(), c.end(), sortByPriority);


    return c;
}

vector<command> game::getSchedCommands(){

    vector<command> c;
    vector<command>::iterator it = mSchedCommands.begin();

    while(it !=mSchedCommands.end()){
        if(it->execAt == ofGetFrameNum()){
            c.push_back(*it);
            it = mSchedCommands.erase(it); //now erase the old reference
        }else{
            ++it;
        }
    }

    sort(c.begin(), c.end(), sortByPriority);

    return c;
}

void game::addSchedCommand(command c){

    mSchedCommands.push_back(c);
}

void game::clearSchedCommands(){
    mSchedCommands.clear();
}
