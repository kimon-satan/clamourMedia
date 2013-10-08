#ifndef PRESETSTORE_H
#define PRESETSTORE_H

#include "clamourNode.h"
#include "zone.h"

class presetStore
{
    public:

    static map< string, clamourNode> nodePresets;
    static map< string, zone> zonePresets;

    protected:
    private:
};

#endif // PRESETSTORE_H
