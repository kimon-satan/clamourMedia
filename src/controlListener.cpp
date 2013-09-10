//
//  controlListener.cpp
//  clamourMedia
//
//  Created by Simon Katan on 06/09/2013.
//
//

#include "controlListener.h"

void controlListener::setup(){
    
    isMouseDown = false;
    
    //populate indexes
    //eventually find this out by message to meteor
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            
            string t_index = ofToString(char(65 + j)) + "_" + ofToString(i + 1);
            mPlayerIndexes.push_back(t_index);
            
        }
    }
    
    
    mOscManager = ofPtr<oscManager>(new oscManager());
    mNodeManager = ofPtr<nodeManager>(new nodeManager(mPlayerIndexes));
    mOscManager->setNodeManager(mNodeManager);
       
    mOscManager->sendInit();
    
    setupGames();
    implementStage();
    
    gui = new ofxUICanvas(0,0,ofGetWidth(),ofGetHeight());
	gui->addWidgetDown(new ofxUILabel("CLAMOUR CONTROL PANEL", OFX_UI_FONT_LARGE));
    
    //stage increment
    gui->addWidgetDown(new ofxUITextArea("CURRENT_STAGE", ofToString(mCurrentGame->getCurrentStage(),1), 50));
    ofxUILabelButton * spb = (ofxUILabelButton *)gui->addWidgetRight(new ofxUILabelButton( "STAGE_PLUS", true ,25));
    spb->setLabelText("+");

    ofAddListener(gui->newGUIEvent,this,&controlListener::guiEvent);
 

}


void controlListener::setupGames(){

    //a temporary method before XML interface is implemented
    
    ofPtr<game> gm = ofPtr<game>(new game());
    gm->setName("hello world");
    ofPtr<group> grp = ofPtr<group>(new group());
    grp->name = "allPlyrGrp";
    grp->indexes = mPlayerIndexes; // all players
  
    //will eventually need a safety for double allocation 
    mGroups[grp->name] = grp;
    
    command cmd;
    
    cmd.targets.push_back(grp->name);
    cmd.stage = 0;
    cmd.priority = 0;
    cmd.mCommand = "SET_CONTROL";
    cmd.intParams["CONTROL_TYPE"] = 0;
    
    gm->addCommand(cmd);
    
    //next command
    
    cmd.stage = 0;
    cmd.priority = 1;
    cmd.mCommand = "SET_TEXT";
    cmd.stringParams["TEXT"] = "Welcome to Clamour";
    
    gm->addCommand(cmd);
    
    //on a new stage
    
    cmd.stage = 1;
    cmd.priority = 0;
    cmd.mCommand = "SET_TEXT";
    cmd.stringParams["TEXT"] = "This is a new piece of text";

    gm->addCommand(cmd);
    
    mGames.push_back(gm);
    
    mCurrentGame = gm;
    

}

void controlListener::update(ofxFenster *f){

    mOscManager->update();
    mNodeManager->updateNodes();

}

void controlListener::draw(){

    ofBackground(100);
    ofSetColor(255);
    
    gui->draw();
    
   /*  string buf;
     buf = "listening for osc messages on port: " + ofToString(METEOR_IN_PORT);
     ofDrawBitmapString(buf, 10, 20);
     ofDrawBitmapString("currentControl: " + ofToString(currentControl,0), 10, 40);
    
     vector<string> msgs = mOscManager->getMsgStrings();
     
     for(int i = 0; i < msgs.size(); i++){
         ofDrawBitmapString(msgs[i], 10, 40 + 15 * i);
     };*/
     
    

}

ofPtr<nodeManager> controlListener::getNodeManager(){

    return mNodeManager;
}

void controlListener::setDisplayRef (ofxFenster * f){

    displayFenster = f;
}

void controlListener::keyPressed(int key, ofxFenster* window){
    
    
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
        
        if(mCurrentGame){
            
            mCurrentGame->incrementStage();
            implementStage();
            //update gui element
            ofxUITextArea * t = (ofxUITextArea *)gui->getWidget("CURRENT_STAGE");
            t->setTextString(ofToString(mCurrentGame->getCurrentStage(),1));
        }
        
    }else{
        
        cout << "guiEvent \n";
    
    }
  

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
        
        }
        
        
    }
    
    

}



void controlListener::setText(group g){

}



