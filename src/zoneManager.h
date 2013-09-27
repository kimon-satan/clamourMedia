#ifndef ZONEMANAGER_H
#define ZONEMANAGER_H

#include "zone.h"

class zoneManager
{
    public:
        zoneManager();
        virtual ~zoneManager();

        void update(map<string, ofPtr<clamourNode> > tNodes);

        void createZone(string name);
        void createZone(zone z);

        void hideZone(string name);
        void showZone(string name);
        void destroyZone(string name);
        void destroyAllZones();


        map<string, ofPtr<zone> > getZones();

        //could be in a base but not much advantage ?

        void setZoneDrawParam(vector<string> indexes, parameter p);
        void setZoneDrawType(vector<string> indexes, string st);
        void setZoneSoundParam(vector<string> indexes, parameter p);
        void setZoneSoundType(vector<string> indexes, string st);


    private:

        void repellNode(ofPtr<clamourNode> n, ofPtr<zone> z);
        void containNode(ofPtr<clamourNode> n, ofPtr<zone> z);

        bool checkInZone(ofPtr<clamourNode> n, ofPtr<zone> z);

        bool getIsRuleMet(ofPtr<zone> z, zoneRule r);

        bool getReaction(ofPtr<zone> z);
        void makeReaction(ofPtr<zone> z);

        map <string, ofPtr<zone> > mZones;

        soundDictionary mSoundDictionary;
};

#endif // ZONEMANAGER_H
