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


    private:

        map <string, ofPtr<zone> > mZones;
};

#endif // ZONEMANAGER_H
