#ifndef XMLLOADER_H
#define XMLLOADER_H

#include "ofxXmlSettings.h"

#include "drawDictionary.h"
#include "soundDictionary.h"
#include "game.h"


class xmlLoader
{
    public:

    static void loadCommands(ofPtr<game> gm, ofxXmlSettings &XML, int stage, int pty = 0);
    static void parseActions(command &cmd, ofxXmlSettings &XML);
    static void loadZone(zone &z, ofxXmlSettings &XML);
    static void loadRule(zoneRule &r, ofxXmlSettings &XML);
    static void loadReaction(reaction &r, ofxXmlSettings &XML);

    static void loadNode(clamourNode &n, ofxXmlSettings &XML);

    static void loadParam(parameter &p, ofxXmlSettings &XML);
    static void loadParams(vector<parameter> &p, ofxXmlSettings &XML);

    static void loadSound(baseData &bd, ofxXmlSettings &XML);
    static void loadDraw(baseData &bd, ofxXmlSettings &XML);

};

#endif // XMLLOADER_H