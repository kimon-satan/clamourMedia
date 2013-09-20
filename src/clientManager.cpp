#include "clientManager.h"

clientManager::clientManager(vector<string> v)
{
    mPlayerIndexes = v;
}


void clientManager::updateOnlineClients(vector<string> v)
{
    mOnlineClients = v;
}

vector<string> clientManager::getOnlineClients()
{
    return mOnlineClients;
}


bool clientManager::getIsClientOnline(string t_index)
{
    return (find(mOnlineClients.begin(), mOnlineClients.end(), t_index) != mOnlineClients.end());
}

void clientManager::setCtrlIndexes(vector<string> clients, int len)
{

    for(int i = 0; i < clients.size(); i++)
    {
        mCtrlIndexes[clients[i]] = createCtrlIndex(len);
    }

}

string clientManager::getCtrlIndex(string client)
{

    return mCtrlIndexes[client];
}

string clientManager::createCtrlIndex(int len)
{

    string s = "";

    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for(int i = 0; i < len; i++)
    {

        s = s + alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return s;

}

void clientManager::createGroup(vector<string> selectors, string name)
{

    ofPtr<group> grp = ofPtr<group>(new group());
    grp->name = name;
    grp->indexes = mPlayerIndexes; //start with all players

    selectClients(selectors, grp);

    //will eventually need a safety for double allocation
    mGroups[grp->name] = grp;


}

void clientManager::createGroup(vector<string> clients, vector<string> selectors, string name)
{

    ofPtr<group> grp = ofPtr<group>(new group());
    grp->name = name;
    grp->indexes = clients; //start with all players

    selectClients(selectors, grp);

    mGroups[grp->name] = grp;

}

void clientManager::selectClients(vector<string> selectors, ofPtr<group> grp)
{

    for(int selector = 0; selector < selectors.size(); selector++)
    {

        vector<string> t_indexes;

        if(selectors[selector] == "all")
        {
            t_indexes = mPlayerIndexes;
        }
        else if(selectors[selector] == "online")
        {
            t_indexes = mOnlineClients;
        }
        else if(selectors[selector] == "single")
        {

            if(grp->indexes.size() > 1)
            {
                int i = ofRandom(0, grp->indexes.size() - 1);
                t_indexes.push_back(grp->indexes[i]);
            }
            else
            {
                t_indexes = grp->indexes;
            }

        }
        else if(selectors[selector] == "pair")
        {

            if(grp->indexes.size() > 2)
            {
                int first = ofRandom(0, grp->indexes.size() - 2);
                int second = ofRandom(0, grp->indexes.size() - 1);
                if (second == first)second = grp->indexes.size() - 1;

                t_indexes.push_back(grp->indexes[first]);
                t_indexes.push_back(grp->indexes[second]);

            }
            else
            {
                t_indexes = grp->indexes;
            }

        }

        vector<string>::iterator it;

        for(it = grp->indexes.begin(); it !=grp->indexes.end(); it++)
        {

            if(selectors[selector] == "odd")
            {

                if(ofToInt((*it).substr(2))%2 != 0)
                {
                    t_indexes.push_back((*it));
                }

            }
            else if(selectors[selector] == "even")
            {

                if(ofToInt((*it).substr(2))%2 == 0)
                {
                    t_indexes.push_back((*it));
                }

            }
            else if(selectors[selector] == "right")
            {

                if(ofToInt((*it).substr(2)) <= NUM_SEATS/2)
                {
                    t_indexes.push_back((*it));
                }

            }
            else if(selectors[selector] == "left")
            {

                if(ofToInt((*it).substr(2)) > NUM_SEATS/2 )
                {
                    t_indexes.push_back((*it));
                }

            }
            else if(selectors[selector] == "back")
            {

                string s = (*it).substr(0,1);
                int i = s[0] - 64; //convert into an integer

                if(i > NUM_ROWS/2)
                {
                    t_indexes.push_back((*it));
                }

            }
            else if(selectors[selector] == "front")
            {

                string s = (*it).substr(0,1);
                int i = s[0] - 64; //convert into an integer

                if(i <= NUM_ROWS/2)
                {
                    t_indexes.push_back((*it));
                }
            }
        }

        grp->indexes = t_indexes;

    }

}

ofPtr<group> clientManager::getGroup(string n)
{
    return mGroups[n];
}

clientManager::~clientManager()
{
    //dtor
}
