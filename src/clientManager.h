#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "ofMain.h"
#include "clamourUtils.h"

struct group{

    string name;
    vector<string> indexes;

};

struct selector{

    string sType;
    int seat;
    string row;
    int numPlayers;
    //perhaps more

};

class clientManager
{
    public:
        clientManager(vector<string> vS);
        virtual ~clientManager();

        void updateOnlineClients(vector<string> v);
        vector<string> getOnlineClients();
        bool getIsClientOnline(string t_index);
        void setCtrlIndexes(vector<string> clients, int len);
        string getCtrlIndex(string client);
        string createCtrlIndex(int len);
        void createGroup(vector<selector> selectors, string name);
        void createGroup(vector<string> clients, vector<selector> selectors, string name);
        void createGroup(vector<string> clients, vector<selector> selectors, string name, vector<string> rmvFrmGrps);
        void selectClients(vector<selector> selectors, ofPtr<group> grp);
        ofPtr<group> getGroup(string n);

    private:

        vector<string> mPlayerIndexes;
        vector<string> mOnlineClients;
        map<string, string> mCtrlIndexes;
        map<string, ofPtr<group> > mGroups;

};

#endif // CLIENTMANAGER_H
