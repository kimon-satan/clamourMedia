#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){


	ofSetFrameRate(60);
	ofSetWindowPosition(800, 800);
	ofSetWindowTitle("CLAMOUR CONTROL");
	ofSetVerticalSync(false);

	ofxFensterManager::get()->setupWindow(&mDisplay);

	 for(int i = 0; i < NUM_ROWS; i++){
        for(int j = 0; j < NUM_SEATS; j++){

            string t_index = ofToString(char(65 + j)) + "_" + ofToString(i + 1);
            mPlayerIndexes.push_back(t_index);

        }
    }

    mOscManager = ofPtr<oscManager>(new oscManager());
    mNodeManager = ofPtr<nodeManager>(new nodeManager(mPlayerIndexes));
    mOscManager->setNodeManager(mNodeManager);

    mDisplay.setNodeManager(mNodeManager);


    isMouseDown = false;


    mOscManager->sendInit();

    loadXML();
    mCurrentGame = mGames[0];

    implementStage();

    mGameBrowseIndex = 0;


    setupGUI();

}

void ofApp::setupGUI(){


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

void ofApp::setupTextArea(ofxUIWidget * w){

    w->setColorBack(ofxUIColor(0));
    w->setColorFill(ofxUIColor(0,255,255));
    w->setDrawBack(true);
    w->setPadding(2);

}

void ofApp::loadXML(){

    ofxXmlSettings XML;

    if(XML.loadFile("XMLs/draft.xml")){

        if(XML.pushTag("CLAMOUR_MEDIA")){


            //static groups
            //potentially add subgroups later

            int numGroups = XML.getNumTags("GROUP");

            for(int gp = 0; gp < numGroups; gp++){

                if(XML.pushTag("GROUP", gp)){

                    string action = XML.getValue("ACTION", "create");

                    if(action == "create"){

                        ofPtr<group> grp = ofPtr<group>(new group());
                        grp->name = XML.getValue("NAME", "default");

                        grp->indexes = mPlayerIndexes; //start with all players

                        int numSelect = XML.getNumTags("SELECTOR");

                        vector<string> selectors;

                        for(int sel = 0; sel < numSelect; sel++)selectors.push_back(XML.getValue("SELECTOR", "all", sel));

                        selectClients(selectors, grp);

                        //will eventually need a safety for double allocation
                        mGroups[grp->name] = grp;



                    }

                    XML.popTag(); //GROUP
                }

            }

            int numGames = XML.getNumTags("GAME");

            for(int gm = 0; gm < numGames; gm++){

                if(XML.pushTag("GAME", gm)){

                    ofPtr<game> gm = ofPtr<game>(new game());
                    gm->setName(XML.getValue("NAME", "default"));

                    int numStages = XML.getNumTags("STAGE");

                    for(int stage = 0; stage < numStages; stage ++){

                        if(XML.pushTag("STAGE", stage)){

                            int numPriority = XML.getNumTags("PRIORITY");

                            for(int pty = 0; pty < numPriority; pty++){

                                if(XML.pushTag("PRIORITY", pty)){

                                    loadCommands(gm, XML, stage, pty);

                                    XML.popTag(); //PRIORTY

                                }

                            }

                            if(numPriority == 0 ){

                                loadCommands(gm, XML, stage); //sometimes we don't need priorities

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

    }else{

        cout << "can't find file \n";

    }

}


void ofApp::loadCommands(ofPtr<game> gm, ofxXmlSettings &XML, int stage, int pty){

    int numCmds = XML.getNumTags("COMMAND");

    for(int cmd = 0; cmd < numCmds; cmd++){

        if(XML.pushTag("COMMAND", cmd)){

            command t_cmd;

            int numTargets = XML.getNumTags("TARGET");

            for(int tgt = 0; tgt < numTargets; tgt++){
                t_cmd.targets.push_back(XML.getValue("TARGET", "",tgt));
            }

            t_cmd.stage = stage;
            t_cmd.priority = pty;

            parseActions(t_cmd, XML);

            gm->addCommand(t_cmd);


            XML.popTag(); // COMMAND
        }

    }

}

void ofApp::parseActions(command &cmd, ofxXmlSettings &XML){

    cmd.mCommand = XML.getValue("ACTION", "none");

    if(XML.tagExists("CONTROL_TYPE"))cmd.stringParams["CONTROL_TYPE"] = XML.getValue("CONTROL_TYPE","");
    if(XML.tagExists("TEXT"))cmd.stringParams["TEXT"] = XML.getValue("TEXT","");
    if(XML.tagExists("DRAW_TYPE"))cmd.intParams["DRAW_TYPE"] = XML.getValue("DRAW_TYPE",0);
    if(XML.tagExists("SOUND_TYPE"))cmd.stringParams["SOUND_TYPE"] = XML.getValue("SOUND_TYPE","");
    if(XML.tagExists("SELECTORS"))cmd.stringParams["SELECTORS"] = XML.getValue("SELECTORS","");

    if(XML.tagExists("PARAM"))cmd.stringParams["PARAM"] = XML.getValue("PARAM", "");
    if(XML.tagExists("MIN_VAL"))cmd.floatParams["MIN_VAL"] = XML.getValue("MIN_VAL", 0.0);
    if(XML.tagExists("MAX_VAL"))cmd.floatParams["MAX_VAL"] = XML.getValue("MAX_VAL", 1.0);
    if(XML.tagExists("ABS_VAL"))cmd.floatParams["ABS_VAL"] = XML.getValue("ABS_VAL", 0.0);
    if(XML.tagExists("MAP_TYPE"))cmd.intParams["MAP_TYPE"] = XML.getValue("MAP_TYPE",0);

    if(XML.tagExists("ATTACK_SECS"))cmd.floatParams["ATTACK_SECS"] = XML.getValue("ATTACK_SECS", 0.0);
    if(XML.tagExists("DECAY_SECS"))cmd.floatParams["DECAY_SECS"] = XML.getValue("DECAY_SECS", 0.0);
    if(XML.tagExists("NAME"))cmd.stringParams["NAME"] = XML.getValue("NAME", "default");

}

void ofApp::selectClients(vector<string> selectors, ofPtr<group> grp){

    for(int selector = 0; selector < selectors.size(); selector++){

        vector<string> t_indexes;

        if(selectors[selector] == "all"){
            t_indexes = mPlayerIndexes;
        }else if(selectors[selector] == "online"){
            t_indexes = mNodeManager->getOnlineClients();
        }else if(selectors[selector] == "single"){

            if(grp->indexes.size() > 1){
                int i = ofRandom(0, grp->indexes.size() - 1);
                t_indexes.push_back(grp->indexes[i]);
            }else{
                t_indexes = grp->indexes;
            }

        }else if(selectors[selector] == "pair"){

            if(grp->indexes.size() > 2){
                int first = ofRandom(0, grp->indexes.size() - 2);
                int second = ofRandom(0, grp->indexes.size() - 1);
                if (second == first)second = grp->indexes.size() - 1;

                t_indexes.push_back(grp->indexes[first]);
                t_indexes.push_back(grp->indexes[second]);

            }else{
                t_indexes = grp->indexes;
            }

        }

        vector<string>::iterator it;

        for(it = grp->indexes.begin(); it !=grp->indexes.end(); it++){

            if(selectors[selector] == "odd"){

                if(ofToInt((*it).substr(2))%2 != 0){
                    t_indexes.push_back((*it));
                }

            }else if(selectors[selector] == "even"){

                if(ofToInt((*it).substr(2))%2 == 0){
                    t_indexes.push_back((*it));
                }

            }else if(selectors[selector] == "right"){

                if(ofToInt((*it).substr(2)) <= NUM_SEATS/2){
                    t_indexes.push_back((*it));
                }

            }else if(selectors[selector] == "left"){

                if(ofToInt((*it).substr(2)) > NUM_SEATS/2 ){
                    t_indexes.push_back((*it));
                }

            }else if(selectors[selector] == "back"){

                string s = (*it).substr(0,1);
                int i = s[0] - 64; //convert into an integer

                if(i > NUM_ROWS/2){
                    t_indexes.push_back((*it));
                }

            }else if(selectors[selector] == "front"){

                string s = (*it).substr(0,1);
                int i = s[0] - 64; //convert into an integer

                if(i <= NUM_ROWS/2){
                    t_indexes.push_back((*it));
                }
            }
        }

        grp->indexes = t_indexes;

    }



}
void ofApp::update(){

    ofBackground(100);
    mOscManager->update();
    mNodeManager->updateNodes();

    ofxUITextArea * t;
    t = (ofxUITextArea *)gui->getWidget("METEOR_OSC_IN");
    t->setTextString(mOscManager->getMsgString(CLAMOUR_MSG_METEOR_IN));
    t = (ofxUITextArea *)gui->getWidget("METEOR_OSC_OUT");
    t->setTextString(mOscManager->getMsgString(CLAMOUR_MSG_METEOR_OUT));
    t = (ofxUITextArea *)gui->getWidget("SC_OSC_OUT");
    t->setTextString(mOscManager->getMsgString(CLAMOUR_MSG_SC_OUT));

}

//--------------------------------------------------------------
void ofApp::draw(){

	gui->draw();
}

void ofApp::guiEvent(ofxUIEventArgs &e){

    //this could also be made into OSC for remote control

    string name = e.widget->getName();

    //only trigger buttons once
    //only trigger buttons once
    if((e.widget->getKind() == OFX_UI_WIDGET_BUTTON || e.widget->getKind() == OFX_UI_WIDGET_LABELBUTTON)
       && ! isMouseDown)return;

    if(name == "STAGE_PLUS"){

        if(!mCurrentGame)return;

        mCurrentGame->incrementStage();
        implementStage();
        updateGUIElements();


    }else if(name == "GAME_PLUS"){

        mGameBrowseIndex = min((int)mGames.size()-1, mGameBrowseIndex + 1);
        updateGUIElements();

    }else if(name == "GAME_MINUS"){

        mGameBrowseIndex = max(0, mGameBrowseIndex - 1);
        updateGUIElements();

    }else if(name == "GAME_SELECT"){

        mDisplay.reset();
        mCurrentGame.reset(); //reset the pointer
        mCurrentGame = mGames[mGameBrowseIndex];
        if(!mCurrentGame)return;
        mCurrentGame->reset();
        implementStage();
        updateGUIElements();

    }else if(name == "GAME_RESET"){

        if(!mCurrentGame)return;
        mCurrentGame->reset();
        mDisplay.reset();
        implementStage();
        updateGUIElements();

    }else{



    }



}

void ofApp::updateGUIElements(){

    ofxUITextArea * t;

    t = (ofxUITextArea *)gui->getWidget("CURRENT_GAME");
    t->setTextString(mCurrentGame->getName());
    t = (ofxUITextArea *)gui->getWidget("ALL_GAMES");
    t->setTextString(mGames[mGameBrowseIndex]->getName());
    t = (ofxUITextArea *)gui->getWidget("CURRENT_STAGE");
    t->setTextString(ofToString(mCurrentGame->getCurrentStage(),1));


}

void ofApp::implementStage(){

     vector<command> tComms = mCurrentGame->getStageCommands();
    //now apply the commands

    for(int i = 0; i < tComms.size(); i++){

        //unpack the groups into individual clients
        vector<string> clients;

        for(int j = 0; j < tComms[i].targets.size(); j++){

            ofPtr<group> tg;
            tg = mGroups[tComms[i].targets[j]];

            if(tg){

                for(int k = 0; k < tg->indexes.size(); k++){
                    clients.push_back(tg->indexes[k]);
                }
                // add all the clients
            }else{

                //it must be an individual client
                //TODO need to check that the client is online
                clients.push_back(tComms[i].targets[j]);

            }

        }

        //get rid of any double entries
        vector<string>::iterator it;

        sort(clients.begin(),clients.end());

        it = unique(clients.begin(), clients.end());

        if(it != clients.end())clients.erase(it);


        //now carry out the command

        if(tComms[i].mCommand == "SET_CONTROL"){


            if(tComms[i].stringParams.find("TEXT") != tComms[i].stringParams.end()){

                mOscManager->setControl(clients, tComms[i].stringParams["CONTROL_TYPE"], tComms[i].stringParams["TEXT"]);

            }else{

                mOscManager->setControl(clients, tComms[i].stringParams["CONTROL_TYPE"]);

            }

        }else if(tComms[i].mCommand == "SET_TEXT"){

            mOscManager->setText(clients, tComms[i].stringParams["TEXT"]);

        }else if(tComms[i].mCommand == "SET_DRAW_TYPE"){

            mNodeManager->setNodeDrawType(clients, tComms[i].intParams["DRAW_TYPE"]);

        }else if(tComms[i].mCommand == "SET_DRAW_PARAM"){

            parameter p(tComms[i].stringParams["PARAM"], tComms[i].floatParams["MIN_VAL"], tComms[i].floatParams["MAX_VAL"], tComms[i].floatParams["ABS_VAL"], (mapType)tComms[i].intParams["MAP_TYPE"]);
            mNodeManager->setNodeDrawParam(clients, p);

        }else if(tComms[i].mCommand == "SET_SOUND_TYPE"){

            //turn off nodes -- not clear that responsibility should rest here
            mNodeManager->switchOffNodes(clients);
            mNodeManager->setNodeSoundType(clients, tComms[i].stringParams["SOUND_TYPE"]);

        }else if(tComms[i].mCommand == "SET_SOUND_PARAM"){

            //does this require the reset of the node as well ?

            parameter p(tComms[i].stringParams["PARAM"], tComms[i].floatParams["MIN_VAL"], tComms[i].floatParams["MAX_VAL"], tComms[i].floatParams["ABS_VAL"], (mapType)tComms[i].intParams["MAP_TYPE"]);
            mNodeManager->setNodeSoundParam(clients, p);

        }else if(tComms[i].mCommand == "ADD_TITLE"){

            title t;

            t.text = tComms[i].stringParams["TEXT"];
            if(tComms[i].floatParams.find("ATTACK_SECS") != tComms[i].floatParams.begin())t.att_secs = tComms[i].floatParams["ATTACK_SECS"];
            if(tComms[i].floatParams.find("DECAY_SECS") != tComms[i].floatParams.begin())t.att_secs = tComms[i].floatParams["DECAY_SECS"];

            mDisplay.addTitle(tComms[i].stringParams["NAME"], t);


        }else if(tComms[i].mCommand == "END_TITLE"){

            mDisplay.endTitle(tComms[i].stringParams["NAME"]);

        }else if(tComms[i].mCommand == "NEW_GROUP"){

            ofPtr<group> grp;

            grp = ofPtr<group>(new group());
            grp->indexes = clients;


            grp->name = tComms[i].stringParams["NAME"];

            string s_string = tComms[i].stringParams["SELECTORS"];

            vector<string> selectors = ofSplitString(s_string, ",");

            selectClients(selectors, grp);


            mGroups[grp->name] = grp;

        }



    }


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){




}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

     isMouseDown = true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

     isMouseDown = false;
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}