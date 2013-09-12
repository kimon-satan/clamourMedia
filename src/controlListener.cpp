//
//  controlListener.cpp
//  clamourMedia
//
//  Created by Simon Katan on 06/09/2013.
//
//

#include "controlListener.h"

void controlListener::setup(ofxFenster * f){
    
    ofSetVerticalSync(false); //achieves higher frame rates for dual screening
    
    isMouseDown = false;
    
    //populate indexes
    //eventually find this out by message to meteor
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            
            string t_index = ofToString(char(65 + j)) + "_" + ofToString(i + 1);
            mPlayerIndexes.push_back(t_index);
            
        }
    }
    
    width = f->getWidth();
    height = f->getHeight();
    
    mOscManager = ofPtr<oscManager>(new oscManager());
    mNodeManager = ofPtr<nodeManager>(new nodeManager(mPlayerIndexes));
    mOscManager->setNodeManager(mNodeManager);
       
    mOscManager->sendInit();
    
    setupGames();
    implementStage();
    
    mGameBrowseIndex = 0;
    
    setupGUI();
 
 

}

void controlListener::setupGUI(){

    
    ofxUIWidget * w;
    ofxUILabelButton * lb;
    
    gui = new ofxUICanvas(0,0,width, height);
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
    
    
    ofAddListener(gui->newGUIEvent,this,&controlListener::guiEvent);

}

void controlListener::setupTextArea(ofxUIWidget * w){

    w->setColorBack(ofxUIColor(0));
    w->setColorFill(ofxUIColor(0,255,255));
    w->setDrawBack(true);
    w->setPadding(2);
    
}

void controlListener::setupGames(){

    //a temporary method before XML interface is implemented
    
    /*--------------Game 1 ----------------------*/
    
    ofPtr<game> gm = ofPtr<game>(new game());
    gm->setName("hello world");
    ofPtr<group> grp = ofPtr<group>(new group());
    grp->name = "allPlyrGrp";
    grp->indexes = mPlayerIndexes; // all players
  
    //will eventually need a safety for double allocation 
    mGroups[grp->name] = grp;
    
    {
        command cmd;
        
        cmd.targets.push_back(grp->name);
        cmd.stage = 0;
        cmd.priority = 0;
        cmd.mCommand = "SET_CONTROL";
        cmd.intParams["CONTROL_TYPE"] = 0;
        
        gm->addCommand(cmd);
    }
    
    //next command
    
    {
        command cmd;
        
        cmd.targets.push_back(grp->name);
        cmd.stage = 0;
        cmd.priority = 1;
        cmd.mCommand = "SET_TEXT";
        cmd.stringParams["TEXT"] = "Welcome to Clamour";
        
        gm->addCommand(cmd);
    
    }
    
    //on a new stage
    
    {
        command cmd;
        
        cmd.targets.push_back(grp->name);
    
        cmd.stage = 1;
        cmd.priority = 0;
        cmd.mCommand = "SET_TEXT";
        cmd.stringParams["TEXT"] = "This is a new piece of text";

        gm->addCommand(cmd);
    
    }
    
    mGames.push_back(gm);
    
    mCurrentGame = gm;
    
    /*-----------------------Game 2 --------------------*/
    
    
    ofPtr<game> gm2 = ofPtr<game>(new game());
    gm2->setName("XY free play");
    
    {
        command cmd;
        
        cmd.targets.push_back(grp->name);
        
        cmd.stage = 0;
        cmd.priority = 0;
        cmd.mCommand = "SET_CONTROL";
        cmd.intParams["CONTROL_TYPE"] = 1;
        
        gm2->addCommand(cmd);
        
    }
    
    {
        command cmd;
        
        cmd.targets.push_back(grp->name);
        
        cmd.stage = 0;
        cmd.priority = 1;
        cmd.mCommand = "SET_DRAW_TYPE";
        cmd.intParams["DRAW_TYPE"] = CLAMOUR_DRAW_FLICKER;
        
        gm2->addCommand(cmd);
        
    }
    
   /* {
        command cmd;
        
        cmd.targets.push_back(grp->name);
        
        cmd.stage = 0;
        cmd.priority = 2;
        cmd.mCommand = "SET_DRAW_PARAM";
        cmd.stringParams["PARAM"] = "flicker";
        cmd.floatParams["MIN_VAL"] = 0;
        cmd.floatParams["MAX_VAL"] = 0.99;
        cmd.floatParams["ABS_VAL"] = 0.5;
        cmd.intParams["MAP_TYPE"] = CLAMOUR_MAP_Y;
        
        gm2->addCommand(cmd);
        
    }*/
    
    {
        command cmd;
        
        cmd.targets.push_back(grp->name);
        
        cmd.stage = 0;
        cmd.priority = 2;
        cmd.mCommand = "SET_SOUND_TYPE";
        cmd.stringParams["SOUND_TYPE"] = "pulseGlitcher";
        
        gm2->addCommand(cmd);
        
    }
    
    {
        command cmd;
        
        cmd.targets.push_back(grp->name);
        
        cmd.stage = 0;
        cmd.priority = 3;
        cmd.mCommand = "SET_SOUND_PARAM";
        cmd.stringParams["PARAM"] = "resFilFreq";
        cmd.floatParams["MIN_VAL"] = 100;
        cmd.floatParams["MAX_VAL"] = 1000;
        cmd.floatParams["ABS_VAL"] = 500;
        cmd.intParams["MAP_TYPE"] = CLAMOUR_MAP_Y;
        
        gm2->addCommand(cmd);
        
    }
    
    
    mGames.push_back(gm2);
    

}

void controlListener::update(ofxFenster *f){

    mOscManager->update();
    mNodeManager->updateNodes();
    
  /*  ofxUITextArea * t;
    t = (ofxUITextArea *)gui->getWidget("METEOR_OSC_IN");
    t->setTextString(mOscManager->getMsgString(CLAMOUR_MSG_METEOR_IN));
    t = (ofxUITextArea *)gui->getWidget("METEOR_OSC_OUT");
    t->setTextString(mOscManager->getMsgString(CLAMOUR_MSG_METEOR_OUT));
    t = (ofxUITextArea *)gui->getWidget("SC_OSC_OUT");
    t->setTextString(mOscManager->getMsgString(CLAMOUR_MSG_SC_OUT)); */
    

}

void controlListener::draw(){

    ofBackground(100);
    ofSetColor(255);
    
    gui->draw();
     
    

}

ofPtr<nodeManager> controlListener::getNodeManager(){

    return mNodeManager;
}

void controlListener::setDisplayRef (ofxFenster * f){

    displayFenster = f;
}

void controlListener::keyPressed(int key, ofxFenster * window){
    
    
    if(key >= 49 && key <= 51){
     
        mOscManager->setAllClients(key - 49);
        mNodeManager->resetNodes();
   
    }

    
}

void controlListener::mousePressed(int x, int y, int button, ofxFenster * window){
    
    isMouseDown = true;
    
}

void controlListener::mouseReleased(int x, int y, int button, ofxFenster * window){
    
    isMouseDown = false;
}



void controlListener::guiEvent(ofxUIEventArgs &e){
    
    string name = e.widget->getName();
    
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
        
        mCurrentGame.reset(); //reset the pointer
        mCurrentGame = mGames[mGameBrowseIndex];
        if(!mCurrentGame)return;
        mCurrentGame->reset();
        implementStage();
        updateGUIElements();
    
    }else if(name == "GAME_RESET"){
        
        if(!mCurrentGame)return;
        mCurrentGame->reset();
        implementStage();
        updateGUIElements();
        
    }else{
        
       
    
    }
  

}

void controlListener::updateGUIElements(){

    ofxUITextArea * t;
 
    t = (ofxUITextArea *)gui->getWidget("CURRENT_GAME");
    t->setTextString(mCurrentGame->getName());
    t = (ofxUITextArea *)gui->getWidget("ALL_GAMES");
    t->setTextString(mGames[mGameBrowseIndex]->getName());
    t = (ofxUITextArea *)gui->getWidget("CURRENT_STAGE");
    t->setTextString(ofToString(mCurrentGame->getCurrentStage(),1));
  

}

void controlListener::implementStage(){
    
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
                clients.push_back(tComms[i].targets[j]);
                cout << "individual client" << endl;
            }
        
        }
        
        //get rid of any double entries
        vector<string>::iterator it;
        
        sort(clients.begin(),clients.end());

        it = unique(clients.begin(), clients.end());
        
        if(it != clients.end())clients.erase(it);
        
        
        //now carry out the command
        
        if(tComms[i].mCommand == "SET_CONTROL"){
            
            mOscManager->setControl(clients, tComms[i].intParams["CONTROL_TYPE"]);
            
        }else if(tComms[i].mCommand == "SET_TEXT"){
            
            mOscManager->setText(clients, tComms[i].stringParams["TEXT"]);
        
        }else if(tComms[i].mCommand == "SET_DRAW_TYPE"){
        
            mNodeManager->setNodeDrawType(clients, tComms[i].intParams["DRAW_TYPE"]);
        
        }else if(tComms[i].mCommand == "SET_DRAW_PARAM"){
        
            parameter p(tComms[i].stringParams["PARAM"], tComms[i].floatParams["MIN_VAL"], tComms[i].floatParams["MAX_VAL"], tComms[i].floatParams["ABS_VAL"], (mapType)tComms[i].intParams["MAP_TYPE"]);
            mNodeManager->setNodeDrawParam(clients, p);
        
        }else if(tComms[i].mCommand == "SET_SOUND_TYPE"){
            
            mNodeManager->setNodeSoundType(clients, tComms[i].stringParams["SOUND_TYPE"]);
            
        }else if(tComms[i].mCommand == "SET_SOUND_PARAM"){
            
            parameter p(tComms[i].stringParams["PARAM"], tComms[i].floatParams["MIN_VAL"], tComms[i].floatParams["MAX_VAL"], tComms[i].floatParams["ABS_VAL"], (mapType)tComms[i].intParams["MAP_TYPE"]);
            mNodeManager->setNodeSoundParam(clients, p);
        }

        
        
    }
    
    

}







