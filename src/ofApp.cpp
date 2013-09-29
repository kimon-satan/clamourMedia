#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{

    //ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetVerticalSync(true);
    //ofSetFrameRate(60);
    ofSetCircleResolution(50);
    ofSetWindowPosition(800, 100);
    ofSetWindowTitle("CLAMOUR CONTROL");


    ofxFensterManager::get()->setupWindow(&mDisplay);

    for(int i = 0; i < NUM_SEATS; i++)
    {
        for(int j = 0; j < NUM_ROWS; j++)
        {

            string t_index = ofToString(char(65 + j)) + "_" + ofToString(i + 1);
            mPlayerIndexes.push_back(t_index);

        }
    }

    soundDictionary::loadSynthDefs();

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



    isMouseDown = false;

    mOscManager->sendInit();

    loadXML();
    mCurrentGame = mGames[0];
    implementStage();

    mGameBrowseIndex = 0;

    setupGUI();

}

void ofApp::setupGUI()
{


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

void ofApp::setupTextArea(ofxUIWidget * w)
{

    w->setColorBack(ofxUIColor(0));
    w->setColorFill(ofxUIColor(0,255,255));
    w->setDrawBack(true);
    w->setPadding(2);

}

void ofApp::loadXML()
{

    //much of this could be moved XmlLoader
    ofxXmlSettings XML;

    if(XML.loadFile("XMLs/draft.xml"))
    {
        if(XML.pushTag("CLAMOUR_MEDIA"))
        {
            //static groups

            int numGroups = XML.getNumTags("GROUP");

            for(int gp = 0; gp < numGroups; gp++)
            {

                if(XML.pushTag("GROUP", gp))
                {

                    string action = XML.getValue("ACTION", "create");

                    if(action == "create")
                    {

                        int numSelect = XML.getNumTags("SELECTOR");

                        //only simple selectors for now ... would require a different XML interface
                        vector<selector> tSels;

                        for(int sel = 0; sel < numSelect; sel++)
                        {
                            selector t;
                            t.sType = XML.getValue("SELECTOR", "all", sel);
                            tSels.push_back(t);
                        }


                        mClientManager->createGroup(tSels, XML.getValue("NAME","default"));

                    }

                    XML.popTag(); //GROUP
                }

            }

            int numGames = XML.getNumTags("GAME");

            for(int gm = 0; gm < numGames; gm++)
            {

                if(XML.pushTag("GAME", gm))
                {

                    ofPtr<game> gm = ofPtr<game>(new game());

                    gm->setName(XML.getValue("NAME", "default"));
                    int numStages = XML.getNumTags("STAGE");

                    for(int stage = 0; stage < numStages; stage ++)
                    {
                        if(XML.pushTag("STAGE", stage))
                        {
                            int numPriority = XML.getNumTags("PRIORITY");

                            for(int pty = 0; pty < numPriority; pty++)
                            {

                                if(XML.pushTag("PRIORITY", pty))
                                {

                                    xmlLoader::loadCommands(gm, XML, stage, pty);
                                    XML.popTag(); //PRIORTY

                                }

                            }

                            if(numPriority == 0 )
                            {
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

    }
    else
    {
        cout << "can't find file \n";
    }

}



void ofApp::update()
{

    ofBackground(100);

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
    t->setTextString(mOscManager->getMsgString(CLAMOUR_MSG_SC_OUT));

}

//--------------------------------------------------------------
void ofApp::draw()
{

    gui->draw();
}

void ofApp::guiEvent(ofxUIEventArgs &e)
{

    //this could also be made into OSC for remote control

    string name = e.widget->getName();

    //only trigger buttons once
    //only trigger buttons once
    if((e.widget->getKind() == OFX_UI_WIDGET_BUTTON || e.widget->getKind() == OFX_UI_WIDGET_LABELBUTTON)
            && ! isMouseDown)return;

    if(name == "STAGE_PLUS")
    {

        if(!mCurrentGame)return;

        mCurrentGame->incrementStage();
        implementStage();
        updateGUIElements();


    }
    else if(name == "GAME_PLUS")
    {

        mGameBrowseIndex = min((int)mGames.size()-1, mGameBrowseIndex + 1);
        updateGUIElements();

    }
    else if(name == "GAME_MINUS")
    {

        mGameBrowseIndex = max(0, mGameBrowseIndex - 1);
        updateGUIElements();

    }
    else if(name == "GAME_SELECT")
    {

        mCurrentGame = mGames[mGameBrowseIndex];
        resetEverything();
        implementStage();
        updateGUIElements();

    }
    else if(name == "GAME_RESET")
    {

        resetEverything();
        implementStage();
        updateGUIElements();

    }
    else
    {



    }



}

void ofApp::updateGUIElements()
{

    ofxUITextArea * t;

    t = (ofxUITextArea *)gui->getWidget("CURRENT_GAME");
    t->setTextString(mCurrentGame->getName());
    t = (ofxUITextArea *)gui->getWidget("ALL_GAMES");
    t->setTextString(mGames[mGameBrowseIndex]->getName());
    t = (ofxUITextArea *)gui->getWidget("CURRENT_STAGE");
    t->setTextString(ofToString(mCurrentGame->getCurrentStage(),1));


}

void ofApp::resetEverything()
{

    mSplashManager->reset();
    if(mCurrentGame)mCurrentGame->reset();
    mNodeManager->switchOffAllNodes();
    mZoneManager->destroyAllZones();

}

void ofApp::unpackClients(vector<string> &clients, command &cmd)
{

    for(int j = 0; j < cmd.targets.size(); j++)
    {

        ofPtr<group> tg;
        tg = mClientManager->getGroup(cmd.targets[j]);

        if(tg)
        {

            for(int k = 0; k < tg->indexes.size(); k++)
            {
                clients.push_back(tg->indexes[k]);
            }
            // add all the clients
        }
        else
        {

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

void ofApp::implementStage()
{

    vector<command> tComms = mCurrentGame->getStageCommands();
    //now apply the commands

    for(int i = 0; i < tComms.size(); i++)
    {
        vector<string> clients;
        unpackClients(clients, tComms[i]);

        //now carry out the command

        if(tComms[i].mCommand == "SET_CONTROL")
        {

            if(tComms[i].stringParams.find("TEXT") != tComms[i].stringParams.end())
            {
                mOscManager->setControl(clients, tComms[i].stringParams["CONTROL_TYPE"], tComms[i].stringParams["TEXT"]);
            }
            else
            {
                mOscManager->setControl(clients, tComms[i].stringParams["CONTROL_TYPE"]);
            }

        }
        else if(tComms[i].mCommand == "SET_TEXT")
        {
            mOscManager->setText(clients, tComms[i].stringParams["TEXT"]);
        }
        else if(tComms[i].mCommand == "SET_NODE_ENV")
        {
            if(tComms[i].floatParams.find("ATTACK_SECS") != tComms[i].floatParams.end())
            {
                mNodeManager->setNodeAttSecs(clients,tComms[i].floatParams["ATTACK_SECS"]);
            }
            if(tComms[i].floatParams.find("DECAY_SECS") != tComms[i].floatParams.end())
            {
                mNodeManager->setNodeDecSecs(clients,tComms[i].floatParams["DECAY_SECS"]);
            }

            //TODO add parameter versions

        }
        else if(tComms[i].mCommand == "SET_NODE_DRAW")
        {
            if(tComms[i].params.size() > 0){
                for(int pi = 0; pi < tComms[i].params.size(); pi ++)mNodeManager->setNodeDrawParam(clients, tComms[i].params[pi]);
            }else{
                mNodeManager->setNodeDraw(clients, tComms[i].mBaseData);
            }
        }
        else if(tComms[i].mCommand == "SET_NODE_SOUND")
        {
            if(tComms[i].params.size() > 0){
                for(int pi = 0; pi < tComms[i].params.size(); pi ++)mNodeManager->setNodeSoundParam(clients, tComms[i].params[pi]);
            }else{
                mNodeManager->setNodeSound(clients, tComms[i].mBaseData);
            }
        }
         else if(tComms[i].mCommand == "SET_ZONE_DRAW")
        {
            if(tComms[i].params.size() > 0){
                for(int pi = 0; pi < tComms[i].params.size(); pi ++)mZoneManager->setZoneDrawParam(clients, tComms[i].params[pi]);
            }else{
                mZoneManager->setZoneDraw(clients, tComms[i].mBaseData);
            }
        }
        else if(tComms[i].mCommand == "SET_ZONE_SOUND")
        {
            if(tComms[i].params.size() > 0){
                for(int pi = 0; pi < tComms[i].params.size(); pi ++)mZoneManager->setZoneSoundParam(clients, tComms[i].params[pi]);
            }else{
                mZoneManager->setZoneSound(clients, tComms[i].mBaseData);
            }
        }
        else if(tComms[i].mCommand == "DISTRIBUTE_NODES")
        {

            bool dimp, posp;

            dimp = (tComms[i].intParams.find("DIM_P") != tComms[i].intParams.end());
            posp = (tComms[i].intParams.find("POS_P") != tComms[i].intParams.end());

            mNodeManager->distributeNodes(clients, tComms[i].stringParams["PATTERN"], tComms[i].floatParams, dimp, posp);

        }
        else if(tComms[i].mCommand == "ADD_TITLE")
        {

            title t;

            t.text = tComms[i].stringParams["TEXT"];
            if(tComms[i].floatParams.find("ATTACK_SECS") != tComms[i].floatParams.end())t.att_secs = tComms[i].floatParams["ATTACK_SECS"];
            if(tComms[i].floatParams.find("DECAY_SECS") != tComms[i].floatParams.end())t.att_secs = tComms[i].floatParams["DECAY_SECS"];

            mSplashManager->addTitle(tComms[i].stringParams["NAME"], t);


        }
        else if(tComms[i].mCommand == "END_TITLE")
        {

            mSplashManager->endTitle(tComms[i].stringParams["NAME"]);

        }
        else if(tComms[i].mCommand == "NEW_GROUP")
        {

            if(tComms[i].selectors.size() == 0)
            {
                string s_string = tComms[i].stringParams["SELECTORS"];
                vector<string> tSelNames = ofSplitString(s_string, ",");

                for(int ts = 0; ts < tSelNames.size(); ts++)
                {
                    selector t;
                    t.sType = tSelNames[ts];
                    tComms[i].selectors.push_back(t);
                }
            }

            if(tComms[i].stringParams.find("RMV_FROM") != tComms[i].stringParams.end())
            {
                vector<string> rNames = ofSplitString(tComms[i].stringParams["RMV_FROM"], ",");
                mClientManager->createGroup(clients, tComms[i].selectors, tComms[i].stringParams["NAME"], rNames);
            }
            else
            {
                mClientManager->createGroup(clients, tComms[i].selectors, tComms[i].stringParams["NAME"]);
            }
        }
        else if(tComms[i].mCommand == "SET_NODE"){

        }
        else if(tComms[i].mCommand == "CREATE_ZONE")
        {
            mZoneManager->createZone(tComms[i].mZone);
        }
        else if(tComms[i].mCommand == "DESTROY_ZONE")
        {
            mZoneManager->destroyZone(tComms[i].stringParams["NAME"]);
        }



    }


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{




}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y )
{

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{

    isMouseDown = true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{

    isMouseDown = false;
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{

}
