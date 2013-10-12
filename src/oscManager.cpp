//
//  oscManager.cpp
//  clamourMedia
//
//  Created by Simon Katan on 07/09/2013.
//
//

#include "oscManager.h"

ofxOscSender oscManager::SCsender;

oscManager::oscManager() {

    receiver.setup(METEOR_IN_PORT);
    sender.setup("192.168.1.200", METEOR_OUT_PORT);


    for(int i = 0; i < 3; i ++) {
        vector<string> s;
        msg_strings.push_back(s);
        msg_string_count.push_back(0);
    }

}

void oscManager::updateInMessages() {

    for(int i = 0; i < 3; i ++) {
        msg_string_count[i] = 0;
    }

    // check for waiting messages
    while(receiver.hasWaitingMessages()) {

        ofxOscMessage m;
        receiver.getNextMessage(&m);

        logMessages(m, CLAMOUR_MSG_METEOR_IN);

        vector<string> s;
        s = ofSplitString(m.getAddress(), "/");
        string t_index;


        if(s[1] == "node") {

            string row = m.getArgAsString(0);
            int seat = (int)m.getArgAsFloat(1);
            t_index = row + "_" + ofToString(seat);
            if(m.getArgAsString(2) != pClientManager->getCtrlIndex(t_index))continue; //ignore message if not from the current control

        }


        if(m.getAddress() == "/node/position") {

            float x = m.getArgAsFloat(3);
            float y = m.getArgAsFloat(4);
            string movType = m.getArgAsString(5);
            ofPtr<clamourNode> n = pNodeManager->getNode(t_index);

            if(n->getIsFiring()) {

                if(movType == "drag" || movType == "drag_c") {
                    pNodeManager->shiftNodePosition(t_index, x, y);
                } else {
                    pNodeManager->updateNodePosition(t_index, x, y);

                }


            }else{

                if(movType == "drag") {

                    pNodeManager->switchOnNode(t_index);
                    pNodeManager->beginShift(t_index, x, y);

                } else {

                    //normal xy stuff
                    pNodeManager->switchOnNode(t_index, x, y);
                }



            }



        } else if(m.getAddress() == "/node/on") {

            //exclude messages from

            float x = m.getArgAsFloat(3);
            float y = m.getArgAsFloat(4);
            string movType = m.getArgAsString(5);

            if(movType == "drag") {

                pNodeManager->switchOnNode(t_index);
                pNodeManager->beginShift(t_index, x, y);



            } else {

                //normal xy stuff
                pNodeManager->switchOnNode(t_index, x, y);
            }


        } else if(m.getAddress() == "/node/startDrag") {

            //for when node is permanently on
            float x = m.getArgAsFloat(3);
            float y = m.getArgAsFloat(4);
            string movType = m.getArgAsString(5);

            pNodeManager->beginShift(t_index, x, y);

        } else if(m.getAddress() == "/node/endDrag") {

            //when node is permanently on
            pNodeManager->getNode(t_index)->setIsDragOn(false);

        } else if(m.getAddress() == "/node/off") {

            pNodeManager->getNode(t_index)->setIsDragOn(false); //prevents shifting call before on call
            pNodeManager->switchOffNode(t_index);

        } else if(m.getAddress() == "/update/onlineUsers") {

            vector<string> v;

            for(int i = 0; i < m.getNumArgs(); i++) {
                v.push_back(m.getArgAsString(i));
            }

            pClientManager->updateOnlineClients(v);

        }

    }










}

void oscManager::updateOutMessages() {

    //TODO put back into various classes and lose flags

    //stop the synths for recently turned off nodes
    map<string, ofPtr<clamourNode> > t_nodes = pNodeManager->getNodes();
    map<string, ofPtr<clamourNode> >::iterator it;

    it = t_nodes.begin();

    while(it != t_nodes.end()) {
        if(it->second->getChanged() == CLAMOUR_ON_OFF) {


            if(it->second->getIsFired()) {
                scMessenger::startSynth(it->second->getName(), *it->second, it->second->getSoundData()); //a dumb hack
            } else {
                scMessenger::stopSynth(it->second->getName(), it->second->getSoundData());
            }


        } else if(it->second->getChanged() == CLAMOUR_SOUND) {

            if(it->second->getIsFiring()) {
                scMessenger::stopSynth(it->second->getName(), it->second->getSoundData());
                scMessenger::startSynth(it->second->getName(), *it->second, it->second->getSoundData());
            }

        } else if(it->second->getChanged() == CLAMOUR_POSITION) {

            scMessenger::updateSynth(it->second->getName(), it->second->getSoundData());

        }

        it->second->setChanged(CLAMOUR_NONE);

        ++it;
    }


    map<string, ofPtr<baseZode> > t_synths = pSplashManager->getSynths();
    map<string, ofPtr<baseZode> >::iterator sit = t_synths.begin();

    while(sit != t_synths.end()) {

        if(sit->second->getChanged() == CLAMOUR_ON_OFF) {
            sit->second->setChanged(CLAMOUR_NONE);

            if(sit->second->getIsFiring()) {

                //send an osc to supercollider
                scMessenger::startSynth(sit->second->getName(), *sit->second, sit->second->getSoundData());
                if(sit->second->getEnvType() == CLAMOUR_AR) {
                    sit->second->setIsFiring(false);
                }

            } else {
                scMessenger::stopSynth(sit->second->getName(), sit->second->getSoundData());
                sit->second->setIsFiring(false);
                //send a stop osc
            }


        }

        ++sit;
    }

}

void oscManager::sendBundle() {

    //send the client's outbundle to Meteor if it has messages waiting

    //nasty dropped message hack ... send five times ugh
    map<string, cBundle>::iterator osc_it;
    bool printed = false;
    int counter;

    for(osc_it = outBundle.begin(); osc_it != outBundle.end(); osc_it++) {

        if(osc_it->second.bundle.getMessageCount() > 0) {

            if(ofGetFrameNum()%10 == osc_it->second.sendNum){
                sender.sendBundle(osc_it->second.bundle);
                osc_it->second.sendCount += 1;
                if(osc_it->second.sendCount == 10)osc_it->second.bundle.clear();
                counter ++;
            }

        }


    }



}


void oscManager::logMessages(ofxOscMessage m, int mt) {

    if(msg_string_count[mt] >= NUM_MSG_STRINGS)return; //for when there's loads of messages

    string msg_string;
    msg_string = m.getAddress();
    msg_string += ": ";

    for(int i = 0; i < m.getNumArgs(); i++) {


        // display the argument - make sure we get the right type
        if(m.getArgType(i) == OFXOSC_TYPE_INT32) {
            msg_string += ofToString(m.getArgAsInt32(i));
        } else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT) {
            msg_string += ofToString(m.getArgAsFloat(i));
        } else if(m.getArgType(i) == OFXOSC_TYPE_STRING) {
            msg_string += m.getArgAsString(i);
        } else {
            msg_string += "unknown";
        }

        msg_string += ", ";
    }

    // add to the list of strings to the log
    msg_strings[mt].push_back( msg_string.substr(0,50)); //maximum 50 chars
    msg_string_count[mt] += 1;


    //make space for new messages
    if(msg_strings[mt].size() > NUM_MSG_STRINGS)msg_strings[mt].erase(msg_strings[mt].begin());

}


void oscManager::setNodeManager(ofPtr<nodeManager> p) {
    pNodeManager = p;
}

void oscManager::setClientManager(ofPtr<clientManager> p) {
    pClientManager =p;
}

void oscManager::setZoneManager(ofPtr<zoneManager> p) {
    pZoneManager = p;

}

void oscManager::setSplashManager(ofPtr<splashManager> p) {
    pSplashManager = p;
}

string oscManager::getMsgString(int mt) {

    string s;

    for(int i = 0; i < msg_strings[mt].size(); i++) {

        s += msg_strings[mt][i];
        s += "\n\n";

    }

    return s;

}

//--------------------------Meteor Messages-----------------------------//

//messages to Meteor need to bundled because of concurrency


void oscManager::setControl(vector<string> clients, string control) {

    pClientManager->setCtrlIndexes(clients, 5);

    for(int i = 0; i < clients.size(); i++) {

        //makes more sense in ofApp
        pNodeManager->wakeupNode(clients[i]);

        if(control == "XY_CONT" || control == "DRAG_CONT" || control == "JOY_CONT") {
            pNodeManager->switchOnNode(clients[i]);
        } else {
            pNodeManager->switchOffNode(clients[i]);
        }


        ofxOscMessage m;

        m.setAddress("/newControl");
        m.addStringArg(clients[i]);
        m.addStringArg(control);
        m.addStringArg("_");
        m.addStringArg(pClientManager->getCtrlIndex(clients[i]));

        logMessages(m, CLAMOUR_MSG_METEOR_OUT);

        sender.sendMessage(m);

    }

}

void oscManager::setControl(vector<string> clients, string control, string text) {

    pClientManager->setCtrlIndexes(clients, 5);
    pNodeManager->switchOffNodes(clients); //this is definitely a text control

    for(int i = 0; i < clients.size(); i++) {

        ofxOscMessage m;

        m.setAddress("/newControl");
        m.addStringArg(clients[i]);
        m.addStringArg(control);
        m.addStringArg(text);
        m.addStringArg(pClientManager->getCtrlIndex(clients[i]));

        logMessages(m, CLAMOUR_MSG_METEOR_OUT);

        sender.sendMessage(m);
        addToBundle(clients[i], m);

    }

}


void oscManager::setText(vector<string> clients, string text) {


    for(int i = 0; i < clients.size(); i++) {
        ofxOscMessage m;

        m.setAddress("/newText");
        m.addStringArg(clients[i]);
        m.addStringArg(text);

        logMessages(m, CLAMOUR_MSG_METEOR_OUT);

        sender.sendMessage(m);
        addToBundle(clients[i], m);

    }

}

void oscManager::addToBundle(string index, ofxOscMessage m) {

    static int sendNumCount = 0;

    sendNumCount = (sendNumCount + 1)%10;

    if(outBundle.find(index) != outBundle.end()) {
        outBundle[index].bundle.clear();
        outBundle[index].bundle.addMessage(m);
        outBundle[index].sendCount = 0;
        outBundle[index].sendNum = sendNumCount;
    } else {
        ofxOscBundle b;
        cBundle c;
        c.bundle = b;
        c.sendNum = outBundle[index].sendNum = sendNumCount;
        outBundle[index] = c;
        outBundle[index].bundle.addMessage(m);
    }

}





