#ifndef ZONEMANAGER_H
#define ZONEMANAGER_H

#include "zone.h"
#include "presetStore.h"
#include "nodeManager.h"


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
        void setupRegions();

        void implementComm(vector<string> zTargets, string comm);

        map<string, ofPtr<zone> > getZones();

        //could be in a base but not much advantage ?

        void setZoneDrawParam(vector<string> indexes, parameter &p);
        void setZoneDraw(vector<string> indexes, baseData &bd);
        void setZoneSoundParam(vector<string> indexes, parameter &p);
        void setZoneSound(vector<string> indexes, baseData &bd);

        void setZoneOnRule(vector<string> indexes,zoneRule zr);
        void setZoneOffRule(vector<string> indexes,zoneRule zr);
        void setZoneReactions(vector<string> indexes, vector<reaction> r);
        void setZoneEnv(vector<string> indexes, string et, float att, float dec);
        void setZonePosition(vector<string> indexes, float x,  float y);
        void setZoneSize(vector<string> indexes, float ts);

        vector<string> getAppReactions();


    private:

        void repellNode(ofPtr<clamourNode> n, ofPtr<zone> z);
        void containNode(ofPtr<clamourNode> n, ofPtr<zone> z);

        bool checkInZone(ofPtr<clamourNode> n, ofPtr<zone> z);

        bool getIsRuleMet(ofPtr<zone> z, zoneRule r);

        bool getOnTrig(ofPtr<zone> z);
        bool getOffTrig(ofPtr<zone> z);

        void onReact(ofPtr<zone> z);
        void offReact(ofPtr<zone> z);
        void implementReactions(ofPtr<zone> z, bool isOn);
        void implementReaction(reaction &r, ofPtr<zone> z, bool isReverse = false);

        map <string, ofPtr<zone> > mZones;

        vector<string> appReactions;
        vector<eventComm> mFutureEvents;


};

#endif // ZONEMANAGER_H
