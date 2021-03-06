#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

    //ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetVerticalSync(false); // only when dual screen
    ofSetFrameRate(60);
    ofSetCircleResolution(50);
    ofSetWindowPosition(800, 100);
    ofSetWindowTitle("CLAMOUR CONTROL");
     smallFont.loadFont("fonts/Goulong/Goulong.ttf", 10);

    ofxFensterManager::get()->setupWindow(&mDisplay);

    for(int i = 0; i < NUM_SEATS; i++) {
        for(int j = 0; j < NUM_ROWS; j++) {

            string t_index = ofToString(char(65 + j)) + "_" + ofToString(i + 1);
            mPlayerIndexes.push_back(t_index);

        }
    }

    soundDictionary::loadSynthDefs();
    scMessenger::setup();

    mClientManager = ofPtr<clientManager>(new clientManager(mPlayerIndexes));

    mOscManager = ofPtr<oscManager>(new oscManager());
    mNodeManager = ofPtr<nodeManager>(new nodeManager(mPlayerIndexes));
    mOscManager->setNodeManager(mNodeManager);
    mOscManager->setClientManager(mClientManager);

    mDisplay.setNodeManager(mNodeManager);

    mZoneManager = ofPtr<zoneManager>(new zoneManager());
    mDisplay.setZoneManager(mZoneManager);
    mOscManager->setZoneManager(mZoneManager);

    mSplashManager = ofPtr<splashManager>(new splashManager());
    mDisplay.setSplashManager(mSplashManager);
    mOscManager->setSplashManager(mSplashManager);

    mNodeManager->setupRegions();

    isMouseDown = false;


    loadXML();
    mCurrentGame = mGames[0];
    implementStage();
    mOscManager->sendBundle();

    mGameBrowseIndex = 0;

    setupGUI();


}

void ofApp::setupGUI() {


    ofxUIWidget * w;
    ofxUILabelButton * lb;

    gui = new ofxUICanvas(0,0,450,600);
    gui->addWidgetDown(new ofxUILabel("CLAMOUR CONTROL PANEL", OFX_UI_FONT_LARGE));

    w = gui->addSpacer(1,15);
    w->setVisible(false);

    gui->addWidgetDown(new ofxUILabel("Current game: ", OFX_UI_FONT_SMALL));

    w = gui->addWidgetRight(
            new ofxUITextArea("CURRENT_GAME", mCurrentGame->getName(), 200, 15,0,0, OFX_UI_FONT_SMALL)
        );

    setupTextArea(w);

    gui->addWidgetDown(new ofxUILabel("Browse games: ", OFX_UI_FONT_SMALL));

    w = gui->addWidgetRight(
            new ofxUITextArea("ALL_GAMES", mGames[mGameBrowseIndex]->getName(), 200, 15,0,0, OFX_UI_FONT_SMALL)
        );

    setupTextArea(w);

    lb = (ofxUILabelButton *)gui->addWidgetRight(new ofxUILabelButton( "GAME_PLUS", true ,25));
    lb->setLabelText("+");

    lb = (ofxUILabelButton *)gui->addWidgetRight(new ofxUILabelButton( "GAME_MINUS", true ,25));
    lb->setLabelText("-");

    lb = (ofxUILabelButton *)gui->addWidgetRight(new ofxUILabelButton( "GAME_SELECT", true ,70, 25));
    lb->setLabelText("SELECT");

    gui->addSpacer();

    gui->addWidgetDown(new ofxUILabel("Stage: ", OFX_UI_FONT_SMALL));

    //stage increment
    w = gui->addWidgetRight(
            new ofxUITextArea("CURRENT_STAGE", ofToString(mCurrentGame->getCurrentStage(),1), 25, 15, 0,0 ,OFX_UI_FONT_SMALL)
        );

    setupTextArea(w);

    lb = (ofxUILabelButton *)gui->addWidgetRight(new ofxUILabelButton( "STAGE_PLUS", true ,25));
    lb->setLabelText("+");


    lb = (ofxUILabelButton *)gui->addWidgetRight(new ofxUILabelButton( "STAGE_REPLAY", true ,80));
    lb->setLabelText("REPLAY");

    lb = (ofxUILabelButton *)gui->addWidgetRight(new ofxUILabelButton( "GAME_RESET", true ,70));
    lb->setLabelText("RESET");



    gui->addWidgetDown(new ofxUILabel("Meteor OSC IN: ", OFX_UI_FONT_SMALL));

    w = gui->addWidgetDown(
            new ofxUITextArea("METEOR_OSC_IN", "Meteor OSC in", 400, 100, 0,0 ,OFX_UI_FONT_SMALL)
        );

    setupTextArea(w);

    gui->addWidgetDown(new ofxUILabel("Meteor OSC OUT: ", OFX_UI_FONT_SMALL));

    w = gui->addWidgetDown(
            new ofxUITextArea("METEOR_OSC_OUT", "Meteor OSC out", 400, 100, 0,0 ,OFX_UI_FONT_SMALL)
        );

    setupTextArea(w);

    gui->addWidgetDown(new ofxUILabel("SC OSC OUT: ", OFX_UI_FONT_SMALL));

    w = gui->addWidgetDown(
            new ofxUITextArea("SC_OSC_OUT","SC OSC OUT", 400, 100, 0,0 ,OFX_UI_FONT_SMALL)
        );

    setupTextArea(w);


    ofAddListener(gui->newGUIEvent,this,&ofApp::guiEvent);

}

void ofApp::setupTextArea(ofxUIWidget * w) {

    w->setColorBack(ofxUIColor(0));
    w->setColorFill(ofxUIColor(0,255,255));
    w->setDrawBack(true);
    w->setPadding(2);

}

void ofApp::loadXML() {

    //much of this could be moved XmlLoader
    ofxXmlSettings XML;

    if(XML.loadFile("XMLs/draft.xml")) {
        if(XML.pushTag("CLAMOUR_MEDIA")) {
            //static groups

            int numGroups = XML.getNumTags("GROUP");

            for(int gp = 0; gp < numGroups; gp++) {

                if(XML.pushTag("GROUP", gp)) {

                    int numSelect = XML.getNumTags("SELECTOR");

                    vector<selector> tSels;

                    for(int sel = 0; sel < numSelect; sel++) {

                        if(XML.pushTag("SELECTOR", sel)){
                            selector ts;

                            ts.sType = XML.getValue("S_TYPE","none");
                            ts.row = XML.getValue("ROW", "A");
                            ts.seat = XML.getValue("SEAT", 1);
                            ts.numPlayers = XML.getValue("NUM_P",1);
                            tSels.push_back(ts);
                            XML.popTag();
                        }
                    }

                    mClientManager->createGroup(tSels, XML.getValue("NAME","default"));

                    XML.popTag(); //GROUP
                }

            }

            int numNPresets = XML.getNumTags("NODE_PRESET");

            for(int np = 0; np < numNPresets; np ++) {

                if(XML.pushTag("NODE_PRESET", np)) {

                    clamourNode c;
                    xmlLoader::loadNode(c, XML);
                    c.setName(XML.getValue("NAME", "default"));
                    presetStore::nodePresets[c.getName()] = c;
                    XML.popTag();
                }
            }

            int numZPresets = XML.getNumTags("ZONE_PRESET");

            for(int zp = 0; zp < numZPresets; zp ++) {

                if(XML.pushTag("ZONE_PRESET", zp)) {

                    zone z;
                    xmlLoader::loadZone(z, XML);
                    z.setName(XML.getValue("NAME", "default"));
                    presetStore::zonePresets[z.getName()] = z;
                    XML.popTag();
                }
            }

            int numGames = XML.getNumTags("GAME");

            for(int gm = 0; gm < numGames; gm++) {

                if(XML.pushTag("GAME", gm)) {

                    ofPtr<game> gm = ofPtr<game>(new game());

                    gm->setName(XML.getValue("NAME", "default"));
                    int numStages = XML.getNumTags("STAGE");

                    for(int stage = 0; stage < numStages; stage ++) {
                        if(XML.pushTag("STAGE", stage)) {
                            int numPriority = XML.getNumTags("PRIORITY");

                            for(int pty = 0; pty < numPriority; pty++) {

                                if(XML.pushTag("PRIORITY", pty)) {

                                    xmlLoader::loadCommands(gm, XML, stage, pty);
                                    XML.popTag(); //PRIORTY

                                }

                            }

                            if(numPriority == 0 ) {
                                xmlLoader::loadCommands(gm, XML, stage); //sometimes we don't need priorities
                            }

                            XML.popTag(); //STAGE
                        }
                    }


                    mGames.push_back(gm);
                    XML.popTag(); //GAME

                }
            }

            XML.popTag(); //CLAMOUR_MEDIA
        }

    } else {
        cout << "can't find file \n";
    }

}



void ofApp::update() {

    ofBackground(100);

    implementNodeReactions();
    implementZoneReactions(); //stuff from the previous frame
    implementSchedCommands();

    //get incoming from Meteor
    mOscManager->updateInMessages();

    //now do all the calculations
    mNodeManager->updateNodes();
    mZoneManager->update(mNodeManager->getNodes());
    mSplashManager->update();

    //update superCollider
    mOscManager->updateOutMessages();
    mOscManager->sendBundle();


    ofxUITextArea * t;
    t = (ofxUITextArea *)gui->getWidget("METEOR_OSC_IN");
    t->setTextString(mOscManager->getMsgString(CLAMOUR_MSG_METEOR_IN));
    t = (ofxUITextArea *)gui->getWidget("METEOR_OSC_OUT");
    t->setTextString(mOscManager->getMsgString(CLAMOUR_MSG_METEOR_OUT));
    t = (ofxUITextArea *)gui->getWidget("SC_OSC_OUT");
    t->setTextString(scMessenger::getMsgString());

}

//--------------------------------------------------------------
void ofApp::draw() {

    gui->draw();

    ofSetColor(255);
    smallFont.drawString(ofToString(ofGetFrameRate(),2),20,600);
}

void ofApp::guiEvent(ofxUIEventArgs &e) {

    //this could also be made into OSC for remote control

    string name = e.widget->getName();

    //only trigger buttons once
    //only trigger buttons once
    if((e.widget->getKind() == OFX_UI_WIDGET_BUTTON || e.widget->getKind() == OFX_UI_WIDGET_LABELBUTTON)
            && ! isMouseDown)return;

    if(name == "STAGE_PLUS") {
        incrementStage();

    } else if(name == "GAME_PLUS") {

        mGameBrowseIndex = min((int)mGames.size()-1, mGameBrowseIndex + 1);
        updateGUIElements();

    } else if(name == "GAME_MINUS") {

        mGameBrowseIndex = max(0, mGameBrowseIndex - 1);
        updateGUIElements();

    } else if(name == "GAME_SELECT") {

        mCurrentGame = mGames[mGameBrowseIndex];
        mCurrentGame->reset();
        //resetEverything(); //don't need this
        implementStage();
        updateGUIElements();

    } else if(name == "GAME_RESET") {

        resetEverything();
        implementStage();
        updateGUIElements();

    } else if(name == "STAGE_REPLAY") {

        implementStage();

    }



}

void ofApp::updateGUIElements() {

    ofxUITextArea * t;

    t = (ofxUITextArea *)gui->getWidget("CURRENT_GAME");
    t->setTextString(mCurrentGame->getName());
    t = (ofxUITextArea *)gui->getWidget("ALL_GAMES");
    t->setTextString(mGames[mGameBrowseIndex]->getName());
    t = (ofxUITextArea *)gui->getWidget("CURRENT_STAGE");
    t->setTextString(ofToString(mCurrentGame->getCurrentStage(),1));


}

void ofApp::resetEverything() {

    mSplashManager->reset();
    if(mCurrentGame)mCurrentGame->reset();
    mNodeManager->killAllNodes();
    mZoneManager->destroyAllZones();

}

void ofApp::unpackClients(vector<string> &clients, command &cmd) {

    for(int j = 0; j < cmd.targets.size(); j++) {

        ofPtr<group> tg;
        tg = mClientManager->getGroup(cmd.targets[j]);

        if(tg) {

            for(int k = 0; k < tg->indexes.size(); k++) {
                clients.push_back(tg->indexes[k]);
            }
            // add all the clients
        } else {

            //it must be an individual client
            clients.push_back(cmd.targets[j]);

        }

    }

    //get rid of any double entries
    vector<string>::iterator it;

    sort(clients.begin(),clients.end());

    it = unique(clients.begin(), clients.end());

    if(it != clients.end())clients.erase(it);

}

void ofApp::implementSchedCommands() {

    if(mCurrentGame) {
        vector<command> cmds = mCurrentGame->getSchedCommands();
        for(int i = 0; i < cmds.size(); i++) {
            implementCommand(cmds[i]);
        }
    }

}

void ofApp::implementNodeReactions() {

    map<string, string> r = mNodeManager->getAppReactions();
    map<string, string> :: iterator it = r.begin();

    while(it != r.end()){

        if(it->second == "resetControl"){
            vector<string> s;
            s.push_back(it->first);
            mOscManager->setControl(s, "BIG_TEXT");
        }

        ++it;
    }

}

void ofApp::implementZoneReactions() {

    vector<string> r = mZoneManager->getAppReactions();
    sort(r.begin(), r.end());
    vector<string>::iterator it = unique(r.begin(), r.end());
    if(it != r.end())r.erase(it);

    it = r.begin();

    while(it != r.end()) {
        if(*it == "incrementStage") {
            incrementStage();
            break;
        } else if(*it == "repeatStage") {
            implementStage(true);
            break;
        } else if(*it == "decrementStage") {
            decrementStage();
            break;
        } else if (*it == "muteSynths"){
            mSplashManager->muteAllSynths();
            break;
        }
        ++it;
    }

}

void ofApp::incrementStage() {

    if(!mCurrentGame)return;

   /* if(mCurrentGame->getCurrentStage()
    mGameBrowseIndex = min((int)mGames.size()-1, mGameBrowseIndex + 1);*/

    if(mCurrentGame->getCurrentStage() == mCurrentGame->getNumStages() - 1){
        mGameBrowseIndex = min((int)mGames.size()-1, mGameBrowseIndex + 1);
        mCurrentGame = mGames[mGameBrowseIndex];
        mCurrentGame->reset();
    }else{
        mCurrentGame->incrementStage();
    }
    bool isRepeat = !(mCurrentGame->getCurrentStage() >=  mCurrentGame->getFurthestStage());
    implementStage(isRepeat); //sometimes a repeat - how can this be worked out  ? // game stores highest stage reached
    updateGUIElements();

}

void ofApp::decrementStage() {

    if(!mCurrentGame)return;
    mCurrentGame->decrementStage();
    implementStage(true);
    updateGUIElements();

}

void ofApp::implementStage(bool isRepeat) {

    vector<command> tComms = mCurrentGame->getStageCommands();

    //now apply the commands

    for(int i = 0; i < tComms.size(); i++) {

        if(isRepeat && !tComms[i].isRepeatable) {
            continue; //skip non-repeatable commands on repeats !
        } else {
            implementCommand(tComms[i]);
        }

    }

}

void ofApp::scheduleCommands(command &cmd, vector<string> & clients) {

    bool isSched = true;

    if(cmd.schedType == "doNum") {
        if(cmd.execNum == cmd.totExecs - 1)isSched = false;
    }

    if(cmd.schedType == "eachTarget") {
        if(cmd.execNum == clients.size() - 1)isSched = false;
        string s = clients[cmd.execNum];
        clients.clear();
        clients.push_back(s);
    }

    if(cmd.schedType == "delay") {
        if(cmd.execNum == 1)isSched = false;
    }


    if(isSched) {
        //calculate the next scheduled time for the command
        cmd.interval = cmd.interval_secs * ofGetFrameRate();
        cmd.execAt = cmd.interval + ofGetFrameNum();
        cmd.execNum += 1;
        mCurrentGame->addSchedCommand(cmd);

        //special commands to count num of times and create accels etc will go here
        if(cmd.schedType == "accel")cmd.interval_secs = max(0.1, cmd.interval_secs * 0.95);
        if(cmd.schedType == "decel")cmd.interval_secs = max(0.1, cmd.interval_secs * 1.05);

        vector<command> tComms = mCurrentGame->getStageCommands();

        //find all other schedulable commands from the same stage
        //copy over scheduled exec time
        //push_back to schedlist

        for(int i = 0; i < tComms.size(); i++) {
            if(tComms[i].isSchedulable && tComms[i].schedType == "none") {
                tComms[i].execAt = cmd.execAt;
                mCurrentGame->addSchedCommand(tComms[i]);
            }
        }
    }


}

void ofApp::implementCommand(command &cmd) {


    // if(CLAMOUR_VERBOSE == true)cout << cmd.mCommand << endl;

    vector<string> clients;
    unpackClients(clients, cmd);

    if(cmd.schedType != "none")scheduleCommands(cmd, clients);
    if(cmd.schedType == "delay" && cmd.execAt != ofGetFrameNum())return;

    //now carry out the command

    if(cmd.mCommand == "SET_CONTROL") {

        if(cmd.stringParams.find("TEXT") != cmd.stringParams.end()) {
            mOscManager->setControl(clients, cmd.stringParams["CONTROL_TYPE"], cmd.stringParams["TEXT"]);
        } else {
            mOscManager->setControl(clients, cmd.stringParams["CONTROL_TYPE"]);
        }

    } else if(cmd.mCommand == "SET_TEXT") {
        mOscManager->setText(clients, cmd.stringParams["TEXT"]);
    } else if(cmd.mCommand == "SET_NODE_ENV") {

        if(cmd.floatParams.find("ATTACK_SECS") != cmd.floatParams.end()) {
            mNodeManager->setNodeAttSecs(clients,cmd.floatParams["ATTACK_SECS"]);
        }
        if(cmd.floatParams.find("DECAY_SECS") != cmd.floatParams.end()) {
            mNodeManager->setNodeDecSecs(clients,cmd.floatParams["DECAY_SECS"]);
        }

        //TODO add parameter versions

    } else if(cmd.mCommand == "SET_NODE_DRAW") {
        if(cmd.params.size() > 0) {
            for(int pi = 0; pi < cmd.params.size(); pi ++)mNodeManager->setNodeDrawParam(clients, cmd.params[pi]);
        } else {
            mNodeManager->setNodeDraw(clients, cmd.mBaseData);
        }
    } else if(cmd.mCommand == "SET_NODE_SOUND") {
        if(cmd.params.size() > 0) {
            for(int pi = 0; pi < cmd.params.size(); pi ++)mNodeManager->setNodeSoundParam(clients, cmd.params[pi]);
        } else {
            mNodeManager->setNodeSound(clients, cmd.mBaseData);
        }
    } else if(cmd.mCommand == "SET_ZONE_DRAW") {
        if(cmd.params.size() > 0) {
            for(int pi = 0; pi < cmd.params.size(); pi ++)mZoneManager->setZoneDrawParam(cmd.zTargets, cmd.params[pi]);
        } else {
            mZoneManager->setZoneDraw(clients, cmd.mBaseData);
        }
    } else if(cmd.mCommand == "SET_ZONE_SOUND") {
        if(cmd.params.size() > 0) {
            for(int pi = 0; pi < cmd.params.size(); pi ++)mZoneManager->setZoneSoundParam(cmd.zTargets, cmd.params[pi]);
        } else {
            mZoneManager->setZoneSound(clients, cmd.mBaseData);
        }

    } else if(cmd.mCommand == "UPDATE_ZONE") {


        if(cmd.reactions.size() > 0)mZoneManager->setZoneReactions(cmd.zTargets, cmd.reactions);


    } else if(cmd.mCommand == "DISTRIBUTE_NODES") {

        bool dimp, posp;

        dimp = (cmd.intParams.find("DIM_P") != cmd.intParams.end());
        posp = (cmd.intParams.find("POS_P") != cmd.intParams.end());

        mNodeManager->distributeNodes(clients, cmd.stringParams["PATTERN"], cmd.floatParams, dimp, posp);

    } else if(cmd.mCommand == "ADD_TITLE") {

        title t;

        t.text = cmd.stringParams["TEXT"];
        if(cmd.floatParams.find("ATTACK_SECS") != cmd.floatParams.end())t.att_secs = cmd.floatParams["ATTACK_SECS"];
        if(cmd.floatParams.find("DECAY_SECS") != cmd.floatParams.end())t.dec_secs = cmd.floatParams["DECAY_SECS"];

        mSplashManager->addTitle(cmd.stringParams["NAME"], t);


    } else if(cmd.mCommand == "END_TITLE") {

        mSplashManager->endTitle(cmd.stringParams["NAME"]);

    } else if(cmd.mCommand == "NEW_GROUP") {

        //FIX_ME regular selectors could just be pushed back

        if(cmd.selectors.size() == 0) {

            if(cmd.stringParams.find("SELECTORS") != cmd.stringParams.end()){
                string s_string = cmd.stringParams["SELECTORS"];
                vector<string> tSelNames = ofSplitString(s_string, ",");

                for(int ts = 0; ts < tSelNames.size(); ts++) {
                    selector t;
                    t.sType = tSelNames[ts];
                    cmd.selectors.push_back(t);
                }
            }
        }

        if(cmd.stringParams.find("RMV_FROM") != cmd.stringParams.end()) {
            vector<string> rNames = ofSplitString(cmd.stringParams["RMV_FROM"], ",");
            mClientManager->createGroup(clients, cmd.selectors, cmd.stringParams["NAME"], rNames);
        } else {
            mClientManager->createGroup(clients, cmd.selectors, cmd.stringParams["NAME"]);
        }
    } else if(cmd.mCommand == "SET_NODE") {
        mNodeManager->setNodes(clients, cmd.mNode);
    } else if(cmd.mCommand == "CREATE_ZONE") {
        mZoneManager->createZone(cmd.mZone);
    } else if(cmd.mCommand == "DESTROY_ZONE") {
        mZoneManager->destroyZone(cmd.stringParams["NAME"]);
     } else if(cmd.mCommand == "DESTROY_ALL_ZONES") {
        mZoneManager->destroyAllZones();
    } else if(cmd.mCommand == "START_SYNTH") {
        mSplashManager->addSynth(cmd.sTargets, cmd.mSynth);
    } else if(cmd.mCommand == "STOP_SYNTH") {
        mSplashManager->endSynth(cmd.sTargets);
    } else if(cmd.mCommand == "STOP_ALL_SYNTHS") {
        mSplashManager->stopAllSynths();
    } else if(cmd.mCommand == "MUTE_ALL_SYNTHS") {
        mSplashManager->muteAllSynths();
    } else if(cmd.mCommand == "CLEAR_SCHED"){
        mCurrentGame->clearSchedCommands();
    } else if(cmd.mCommand == "ZONE_COMM") {
        mZoneManager->implementComm(cmd.zTargets, cmd.stringParams["TYPE"]); //needs implementing other parts
    }else if(cmd.mCommand == "INCREMENT_STAGE") {
        incrementStage();
    }else if(cmd.mCommand == "RESET_GAME"){
         resetEverything();
        implementStage();
        updateGUIElements();
    }


}


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {




}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

    isMouseDown = true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

    isMouseDown = false;
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
