#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "ofMain.h"
#include "clamourUtils.h"

struct group{

    string name;
    vector<string> indexes;

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
        void createGroup(vector<string> selectors, string name);
        void createGroup(vector<string> clients, vector<string> selectors, string name);
        void selectClients(vector<string> selectors, ofPtr<group> grp);
        ofPtr<group> getGroup(string n);

    private:

        vector<string> mPlayerIndexes;
        vector<string> mOnlineClients;
        map<string, string> mCtrlIndexes;
        map<string, ofPtr<group> > mGroups;

};

#endif // CLIENTMANAGER_H
