#include "xmlLoader.h"

void xmlLoader::loadCommands(ofPtr<game> gm, ofxXmlSettings &XML, int stage, int pty) {

    int numCmds = XML.getNumTags("COMMAND");

    for(int cmd = 0; cmd < numCmds; cmd++) {

        if(XML.pushTag("COMMAND", cmd)) {

            command t_cmd;

            int numTargets = XML.getNumTags("TARGET");

            for(int tgt = 0; tgt < numTargets; tgt++) {
                t_cmd.targets.push_back(XML.getValue("TARGET", "",tgt));
            }

            int numZTargets = XML.getNumTags("Z_TARGET");

            for(int tgt = 0; tgt < numZTargets; tgt++) {
                t_cmd.zTargets.push_back(XML.getValue("Z_TARGET", "",tgt));
            }

            int numSTargets = XML.getNumTags("S_TARGET");

            for(int tgt = 0; tgt < numSTargets; tgt++) {
                t_cmd.sTargets.push_back(XML.getValue("S_TARGET", "",tgt));
            }

            t_cmd.stage = stage;
            t_cmd.priority = pty;

            if(XML.tagExists("REPEAT"))t_cmd.isRepeatable = XML.getValue("REPEAT", true);
            if(XML.tagExists("SCHED"))t_cmd.isSchedulable = XML.getValue("SCHED", true);
            if(XML.tagExists("SCHED_TYPE"))t_cmd.schedType = XML.getValue("SCHED_TYPE", "none");
            if(XML.tagExists("INTERVAL_SECS"))t_cmd.interval_secs = XML.getValue("INTERVAL_SECS", 0.0);
            if(XML.tagExists("TOT_EXECS"))t_cmd.totExecs = XML.getValue("TOT_EXECS", 1);

            parseActions(t_cmd, XML);

            gm->addCommand(t_cmd);

            XML.popTag(); // COMMAND
        }

    }

}

void xmlLoader::parseActions(command &cmd, ofxXmlSettings &XML) {

    //loads the action and paramters into the command data structure

    cmd.mCommand = XML.getValue("ACTION", "none");

    if(cmd.mCommand == "CREATE_ZONE") {
        if(XML.tagExists("PRESET")) {
            cmd.mZone = presetStore::zonePresets[XML.getValue("PRESET", "default")];
        }
        xmlLoader::loadZone(cmd.mZone, XML);
        return;
    }

    if(cmd.mCommand == "SET_NODE") {
        if(XML.tagExists("PRESET")) {
            cmd.mNode = presetStore::nodePresets[XML.getValue("PRESET", "default")];
        }
        xmlLoader::loadNode(cmd.mNode, XML);
        return;
    }

    if(cmd.mCommand == "SET_NODE_DRAW" || cmd.mCommand == "SET_ZONE_DRAW") {

        if(XML.tagExists("DRAW_TYPE")) {
            xmlLoader::loadDraw(cmd.mBaseData, XML);
        } else {
            xmlLoader::loadParams(cmd.params, XML);
        }
        return;
    }


    if(cmd.mCommand == "SET_NODE_SOUND" || cmd.mCommand == "SET_ZONE_SOUND") {

        if(XML.tagExists("SOUND_TYPE")) {
            xmlLoader::loadSound(cmd.mBaseData, XML);
        } else {
            xmlLoader::loadParams(cmd.params, XML);
        }

        return;

    }

    if(cmd.mCommand == "UPDATE_ZONE"){

        xmlLoader::loadZoneParams(cmd, XML);
    }

    if(cmd.mCommand == "NEW_GROUP") {

        if(XML.tagExists("RMV_FROM"))cmd.stringParams["RMV_FROM"] = XML.getValue("RMV_FROM","");

        int numSelectors = XML.getNumTags("SELECTOR");

        for(int i = 0; i < numSelectors; i ++) {

            if(XML.pushTag("SELECTOR", i)) {

                selector ts;

                ts.sType = XML.getValue("S_TYPE","none");
                ts.row = XML.getValue("ROW", "A");
                ts.seat = XML.getValue("SEAT", 1);
                ts.numPlayers = XML.getValue("NUM_P",1);

                cmd.selectors.push_back(ts);

                XML.popTag();

            }
        }

        if(numSelectors == 0)if(XML.tagExists("SELECTORS"))cmd.stringParams["SELECTORS"] = XML.getValue("SELECTORS","");

    }

    if(cmd.mCommand == "START_SYNTH"){

        xmlLoader::loadSynth(cmd.mSynth, XML);

    }

    //more general stuff

    if(XML.tagExists("CONTROL_TYPE"))cmd.stringParams["CONTROL_TYPE"] = XML.getValue("CONTROL_TYPE","");
    if(XML.tagExists("TEXT"))cmd.stringParams["TEXT"] = XML.getValue("TEXT","");

    if(XML.tagExists("ENV_TYPE"))cmd.stringParams["ENV_TYPE"] = XML.getValue("ENV_TYPE","");
    if(XML.tagExists("ATTACK_SECS"))cmd.floatParams["ATTACK_SECS"] = XML.getValue("ATTACK_SECS", 0.0);
    if(XML.tagExists("DECAY_SECS"))cmd.floatParams["DECAY_SECS"] = XML.getValue("DECAY_SECS", 0.0);
    if(XML.tagExists("X"))cmd.floatParams["X"] = XML.getValue("X", 0.0);
    if(XML.tagExists("Y"))cmd.floatParams["Y"] = XML.getValue("Y", 0.0);
    if(XML.tagExists("SIZE"))cmd.floatParams["SIZE"] = XML.getValue("SIZE", 0.0);
    if(XML.tagExists("NAME"))cmd.stringParams["NAME"] = XML.getValue("NAME", "default");

    if(XML.tagExists("PATTERN"))cmd.stringParams["PATTERN"] = XML.getValue("PATTERN", "default");
    if(XML.tagExists("X"))cmd.floatParams["X"] = XML.getValue("X", 0.5);
    if(XML.tagExists("Y"))cmd.floatParams["Y"] = XML.getValue("Y", 0.5);
    if(XML.tagExists("DIM_P"))cmd.intParams["DIM_P"] = XML.getValue("DIM_P", 0);
    if(XML.tagExists("POS_P"))cmd.intParams["POS_P"] = XML.getValue("POS_P", 0);
    if(XML.tagExists("RADIUS"))cmd.floatParams["RADIUS"] = XML.getValue("RADIUS", 0.25);

}

void xmlLoader::loadZone(zone &z, ofxXmlSettings &XML) {

    z.setName(XML.getValue("NAME", "default"));

    z.setPos_rel(ofVec2f(XML.getValue("X", 0.5), XML.getValue("Y",0.5)));
    ofPath p;

    int numEvents = XML.getNumTags("EVENT");

    for(int i =0; i < numEvents; i ++){
        if(XML.pushTag("EVENT", i)){
            clamourEvent e;
            e.setAttSecs(XML.getValue("ATTACK_SECS", 0.01));
            e.setDecSecs(XML.getValue("DECAY_SECS", 0.2));
            e.setEnvType(XML.getValue("ENV_TYPE", "AR"));
            z.addEvent(e);
            XML.popTag();
        }
    }

    if(XML.tagExists("DRAW_TYPE"))z.setDrawType(XML.getValue("DRAW_TYPE", "debugZone"));

    if(XML.pushTag("DRAW_PARAMS")) {

        vector<parameter> p;
        xmlLoader::loadParams( p ,XML);
        for(int i = 0; i < p.size(); i++)z.setDrawParameter(p[i]);
        XML.popTag();
    }

    pathFactory::createPath(p, z.getDrawData().getShapeType(),
                            XML.getValue("X_DIM",1.0),XML.getValue("Y_DIM", 1.0),
                            z.getDrawData().getParameter("size").abs_val);
    z.setEdgeTemplate(p);

    if(XML.tagExists("ATTACK_SECS"))z.setAttSecs(XML.getValue("ATTACK_SECS", 0.01)); // could be in a param if necessary later
    if(XML.tagExists("DECAY_SECS"))z.setDecSecs(XML.getValue("DECAY_SECS", 0.2));  //
    if(XML.tagExists("ENV_TYPE"))z.setEnvType(XML.getValue("ENV_TYPE", "AR"));

    if(XML.tagExists("SOUND_TYPE"))z.setSoundType(XML.getValue("SOUND_TYPE","brownExploder"));


    if(XML.pushTag("SOUND_PARAMS")) {

        if(XML.tagExists("FILE"))z.setSoundFile(XML.getValue("FILE", "default"));
        vector<parameter> p;
        xmlLoader::loadParams( p ,XML);
        for(int i = 0; i < p.size(); i++)z.setSoundParameter(p[i]);
        XML.popTag();
    }


    if(XML.pushTag("ON_RULE")) {
        zoneRule r;
        xmlLoader::loadRule(r, XML);
        z.setOnRule(r);
        XML.popTag();
    }

    if(XML.pushTag("OFF_RULE")) {
        zoneRule r;
        xmlLoader::loadRule(r, XML);
        z.setOffRule(r);
        XML.popTag();
    }

    int numReacts  = XML.getNumTags("REACT");

    for(int rc = 0; rc < numReacts; rc ++) {
        if(XML.pushTag("REACT", rc)) {
            reaction r;
            xmlLoader::loadReaction(r, XML);
            z.addReaction(r);
            XML.popTag();
        }

    }

    int numSounds = XML.getNumTags("SOUND");

    for(int i = 0; i < numSounds; i ++){

        if(XML.pushTag("SOUND", i)){

            baseData bd;

            string st = XML.getValue("TYPE", "none");
            if(st != "none"){
                bd = soundDictionary::createSoundData(st);
            }else{
                bd.setName(st);
            }

            bd.setEventIndex(XML.getValue("EVENT" , 0));

            //load up the sound and bind to trigger event ...
             if(XML.pushTag("SOUND_PARAMS")) {

                if(XML.tagExists("FILE"))bd.setSoundFile(XML.getValue("FILE", "default"));
                vector<parameter> p;
                xmlLoader::loadParams( p ,XML);

                for(int i = 0; i < p.size(); i++){
                    p[i].index = bd.getParameter(p[i].name).index;
                    p[i].warp = bd.getParameter(p[i].name).warp;
                    bd.setParameter(p[i]);
                }
                XML.popTag();
            }
            //

            z.addSound(bd);

            XML.popTag();
        }

    }


}

void xmlLoader::loadRule(zoneRule &r, ofxXmlSettings &XML) {

    r.ruleType = XML.getValue("TYPE", "MIN_OCCUPANTS");
    r.gtOccupants = XML.getValue("GE", 1);
    r.ltOccupants = XML.getValue("LE", 2);

    int numExc = XML.getNumTags("EXC");
    int numInc = XML.getNumTags("INC");

    for(int i = 0; i < numExc; i ++)r.excDrawTypes.push_back(XML.getValue("EXC", ""));
    for(int i = 0; i < numInc; i ++)r.incDrawTypes.push_back(XML.getValue("INC", ""));

}

void xmlLoader::loadReaction(reaction &r, ofxXmlSettings &XML) {

    r.rType = XML.getValue("TYPE", "closeOutZone");
    r.trig = XML.getValue("TRIG", "ON");
    if(XML.tagExists("PRESET"))r.stringParams["PRESET"] = XML.getValue("PRESET","default");
    if(XML.tagExists("SCALE"))r.floatParams["SCALE"] = XML.getValue("SCALE", 1.0);
    if(XML.tagExists("ENV_INDEX"))r.intParams["ENV_INDEX"] = XML.getValue("ENV_INDEX", 1);
    if(XML.tagExists("DELAY_SECS"))r.floatParams["DELAY_SECS"] = XML.getValue("DELAY_SECS", 0.25);

    int numZs = XML.getNumTags("Z_TARGET");

    for(int i = 0; i < numZs; i++){
        r.zTargets.push_back(XML.getValue("Z_TARGET", "",i));
    }

}

void xmlLoader::loadNode(clamourNode &n, ofxXmlSettings &XML) {

    if(XML.tagExists("ATTACK_SECS"))n.setAttSecs(XML.getValue("ATTACK_SECS", 0.01)); // could be in a param if necessary later
    if(XML.tagExists("DECAY_SECS"))n.setDecSecs(XML.getValue("DECAY_SECS", 0.2));  //
    if(XML.tagExists("ENV_TYPE"))n.setEnvType(XML.getValue("ENV_TYPE", "AR"));

    if(XML.tagExists("SOUND_TYPE"))n.setSoundType(XML.getValue("SOUND_TYPE","none"));
    if(XML.tagExists("DRAW_TYPE"))n.setDrawType(XML.getValue("DRAW_TYPE", "none"));
    if(XML.tagExists("CAN_SLEEP"))n.setCanSleep(XML.getValue("CAN_SLEEP", true));
    if(XML.tagExists("ROTATE")){n.setIsRotate(XML.getValue("ROTATE", true));}

    if(XML.tagExists("SHIFT"))n.setShiftAmount(XML.getValue("SHIFT", 0.2));

    if(XML.pushTag("DRAW_PARAMS")) {

        vector<parameter> p;
        xmlLoader::loadParams( p ,XML);
        for(int i = 0; i < p.size(); i++)n.setDrawParameter(p[i]);
        XML.popTag();
    }


    if(n.getDrawData().getName() != "none"){
        //if statement needed here too
        ofPath p;

        pathFactory::createPath(p, n.getDrawData().getShapeType(),
                                XML.getValue("X_DIM",1.0),XML.getValue("Y_DIM", 1.0),
                                n.getDrawData().getParameter("size").abs_val);

        n.setEdgeTemplate(p);
    }




    if(XML.pushTag("SOUND_PARAMS")) {

        if(XML.tagExists("FILE"))n.setSoundFile(XML.getValue("FILE", "default"));
        vector<parameter> p;
        xmlLoader::loadParams( p ,XML);
        for(int i = 0; i < p.size(); i++) {
            n.setSoundParameter(p[i]);
        }
        XML.popTag();
    }


}

void xmlLoader::loadSynth(baseZode &n, ofxXmlSettings &XML) {

    n.setAttSecs(XML.getValue("ATTACK_SECS", 0.01)); // could be in a param if necessary later
    n.setDecSecs(XML.getValue("DECAY_SECS", 0.01));  //
    n.setEnvType(XML.getValue("ENV_TYPE", "AR"));

    if(XML.tagExists("SOUND_TYPE"))n.setSoundType(XML.getValue("SOUND_TYPE",""));
    if(XML.tagExists("FILE"))n.setSoundFile(XML.getValue("FILE", "default"));

    if(XML.pushTag("SOUND_PARAMS")) {

        vector<parameter> p;
        xmlLoader::loadParams( p ,XML);
        for(int i = 0; i < p.size(); i++) {
            n.setSoundParameter(p[i]);
        }
        XML.popTag();
    }


}

void xmlLoader::loadDraw(baseData &bd, ofxXmlSettings &XML) {

    bd = drawDictionary::createDrawData(XML.getValue("DRAW_TYPE", "default"));
    if(XML.pushTag("DRAW_PARAMS")) {
        vector<parameter> p;
        xmlLoader::loadParams( p ,XML);
        for(int i = 0; i < p.size(); i++)bd.setParameter(p[i]);
        XML.popTag();
    }


}

void xmlLoader::loadSound(baseData &bd, ofxXmlSettings &XML) {
    bd = drawDictionary::createDrawData(XML.getValue("SOUND_TYPE", "default"));
    if(XML.pushTag("SOUND_PARAMS")) {
        if(XML.tagExists("FILE"))bd.setSoundFile(XML.getValue("FILE", "default"));
        vector<parameter> p;
        xmlLoader::loadParams( p ,XML);
        for(int i = 0; i < p.size(); i++)bd.setParameter(p[i]);
        XML.popTag();
    }

}

void xmlLoader::loadParams(vector<parameter> &params, ofxXmlSettings &XML) {

    int numPs = XML.getNumTags("PARAM");

    for(int i = 0; i < numPs; i++) {

        if(XML.pushTag("PARAM", i)) {
            parameter p;
            xmlLoader::loadParam(p, XML);
            params.push_back(p);
            XML.popTag();
        }

    }


}

void xmlLoader::loadParam(parameter &p, ofxXmlSettings &XML) {

    p.name = XML.getValue("NAME", "default");
    p.abs_val = XML.getValue("ABS_VAL", 0.5);
    p.min_val = XML.getValue("MIN_VAL", 0.0);
    p.max_val = XML.getValue("MAX_VAL", 1.0);
    p.map_type = clamourUtils::stringToMapType(XML.getValue("MAP_TYPE", "fixed"));
    p.envIndex = XML.getValue("ENV_INDEX", 0);
    p.slave =  XML.getValue("SLAVE", "");

    if(XML.tagExists("WARP"))p.warp = XML.getValue("WARP", "lin");

}



void xmlLoader::loadZoneParams(command &cmd, ofxXmlSettings &XML){

    if(XML.pushTag("ON_RULE")) {
        xmlLoader::loadRule(cmd.onRule, XML);
        XML.popTag();
    }

    if(XML.pushTag("OFF_RULE")) {
        xmlLoader::loadRule(cmd.offRule, XML);
        XML.popTag();
    }

    int numReacts  = XML.getNumTags("REACT");

    for(int rc = 0; rc < numReacts; rc ++) {
        if(XML.pushTag("REACT", rc)) {
            reaction r;
            xmlLoader::loadReaction(r, XML);
            cmd.reactions.push_back(r);
            XML.popTag();
        }
    }


}

