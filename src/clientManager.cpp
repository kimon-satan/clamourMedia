#include "clientManager.h"

clientManager::clientManager(vector<string> v) {
    mPlayerIndexes = v;
}


void clientManager::updateOnlineClients(vector<string> v) {
    mOnlineClients = v;
}

vector<string> clientManager::getOnlineClients() {
    return mOnlineClients;
}


bool clientManager::getIsClientOnline(string t_index) {
    return (find(mOnlineClients.begin(), mOnlineClients.end(), t_index) != mOnlineClients.end());
}

void clientManager::setCtrlIndexes(vector<string> clients, int len) {

    for(int i = 0; i < clients.size(); i++) {
        mCtrlIndexes[clients[i]] = createCtrlIndex(len);
    }

}

string clientManager::getCtrlIndex(string client) {

    return mCtrlIndexes[client];
}

string clientManager::createCtrlIndex(int len) {

    string s = "";

    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for(int i = 0; i < len; i++) {

        s = s + alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return s;

}

void clientManager::createGroup(vector<selector> selectors, string name) {

    createGroup(mPlayerIndexes, selectors, name); //start with all players

}

void clientManager::createGroup(vector<string> clients, vector<selector> selectors, string name) {

    ofPtr<group> grp = ofPtr<group>(new group());
    grp->name = name;
    grp->indexes = clients;

    if(selectors.size() > 0) {
        selectClients(selectors, grp);
    } //otherwise its just a simple merge

    mGroups[grp->name] = grp;

}

void clientManager::createGroup(vector<string> clients, vector<selector> selectors, string name, vector<string> subFrmGrps) {

    createGroup(clients, selectors, name);

    //now remove any clients from the listed groups
    for(int i = 0; i < subFrmGrps.size(); i ++) {
        //for each sub grp

        vector<string>::iterator it =  mGroups[subFrmGrps[i]]->indexes.begin();

        while(it != mGroups[subFrmGrps[i]]->indexes.end()) { //look through each of the terms in the subFrmGrp

            if(find(mGroups[name]->indexes.begin(), mGroups[name]->indexes.end(),*it)!= mGroups[name]->indexes.end()) { //if it matches one in the new grp

                it = mGroups[subFrmGrps[i]]->indexes.erase(it); //erase it from the subFrm

            } else {

                ++it;
            }
        }

    }


}



void clientManager::selectClients(vector<selector> selectors, ofPtr<group> grp) {

    for(int tSel = 0; tSel < selectors.size(); tSel++) {

        vector<string> t_indexes;

        if(selectors[tSel].sType== "all") {
            t_indexes = mPlayerIndexes;
        } else if(selectors[tSel].sType == "online") {
            t_indexes = mOnlineClients;
        } else if(selectors[tSel].sType == "single") {

            if(grp->indexes.size() > 1) {
                int i = ofRandom(0, grp->indexes.size() - 1);
                t_indexes.push_back(grp->indexes[i]);
            } else {
                t_indexes = grp->indexes;
            }

        } else if(selectors[tSel].sType == "pair") {

            if(grp->indexes.size() > 2) {
                int first = ofRandom(0, grp->indexes.size() - 2);
                int second = ofRandom(0, grp->indexes.size() - 1);
                if (second == first)second = grp->indexes.size() - 1;

                t_indexes.push_back(grp->indexes[first]);
                t_indexes.push_back(grp->indexes[second]);

            } else {
                t_indexes = grp->indexes;
            }

        } else if(selectors[tSel].sType == "random") {

            if(grp->indexes.size() > selectors[tSel].numPlayers) {

                random_shuffle(grp->indexes.begin(), grp->indexes.end());

                for(int i = 0; i < selectors[tSel].numPlayers; i++) {
                    t_indexes.push_back(grp->indexes[i]);
                }

            } else {
                t_indexes = grp->indexes;
            }

        }

        vector<string>::iterator it;

        for(it = grp->indexes.begin(); it !=grp->indexes.end(); it++) {

            if(selectors[tSel].sType == "odd") {

                if(ofToInt((*it).substr(2))%2 != 0) {
                    t_indexes.push_back((*it));
                }

            } else if(selectors[tSel].sType == "even") {

                if(ofToInt((*it).substr(2))%2 == 0) {
                    t_indexes.push_back((*it));
                }

            } else if(selectors[tSel].sType == "right") {

                if(ofToInt((*it).substr(2)) <= NUM_SEATS/2) {
                    t_indexes.push_back((*it));
                }

            } else if(selectors[tSel].sType == "left") {

                if(ofToInt((*it).substr(2)) > NUM_SEATS/2 ) {
                    t_indexes.push_back((*it));
                }

            } else if(selectors[tSel].sType == "back") {

                if(clamourUtils::rowToInt(*it) > NUM_ROWS/2) {
                    t_indexes.push_back((*it));
                }

            } else if(selectors[tSel].sType == "front") {

                if(clamourUtils::rowToInt(*it) <= NUM_ROWS/2) {
                    t_indexes.push_back((*it));
                }

            } else if(selectors[tSel].sType == "le_row") {

                if(clamourUtils::rowToInt(*it) <= clamourUtils::rowToInt(selectors[tSel].row)) {
                    t_indexes.push_back((*it));
                }

            } else if(selectors[tSel].sType == "ge_row") {

                if(clamourUtils::rowToInt(*it) >= clamourUtils::rowToInt(selectors[tSel].row)) {
                    t_indexes.push_back((*it));
                }

            } else if(selectors[tSel].sType == "le_seat") {

                if(ofToInt((*it).substr(2)) <= selectors[tSel].seat ) {
                    t_indexes.push_back((*it));
                }

            } else if(selectors[tSel].sType == "ge_seat") {

                if(ofToInt((*it).substr(2)) >= selectors[tSel].seat ) {
                    t_indexes.push_back((*it));
                }

            }
        }

        grp->indexes = t_indexes;

    }

}

ofPtr<group> clientManager::getGroup(string n) {
    return mGroups[n];
}

clientManager::~clientManager() {
    //dtor
}
