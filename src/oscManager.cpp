//
//  oscManager.cpp
//  clamourMedia
//
//  Created by Simon Katan on 07/09/2013.
//
//

#include "oscManager.h"


oscManager::oscManager() {

    receiver.setup(METEOR_IN_PORT);
    sender.setup("192.168.2.200", METEOR_OUT_PORT);
    SCsender.setup("localhost", SC_OUT_PORT);

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

    //stop the synths for recently turned off nodes
    map<string, ofPtr<clamourNode> > t_nodes = pNodeManager->getNodes();
    map<string, ofPtr<clamourNode> >::iterator it;

    it = t_nodes.begin();

    while(it != t_nodes.end()) {
        if(it->second->getChanged() == CLAMOUR_ON_OFF) {


            if(it->second->getIsFired()) {
                startSynth(it->second);
            } else {
                stopSynth(it->second);
            }


        } else if(it->second->getChanged() == CLAMOUR_SOUND) {

            if(it->second->getIsFiring()) {
                stopSynth(it->second);
                startSynth(it->second);
            }

        } else if(it->second->getChanged() == CLAMOUR_POSITION) {

            updateSynth(it->second);

        }

        it->second->setChanged(CLAMOUR_NONE);

        ++it;
    }

    if(pZoneManager->getZones().size() > 0) {

        map<string, ofPtr<zone> > t_zones = pZoneManager->getZones();
        map<string, ofPtr<zone> >::iterator zit = t_zones.begin();

        while(zit != t_zones.end()) {

            if(zit->second->getChanged() == CLAMOUR_ON_OFF) {

                zit->second->setChanged(CLAMOUR_NONE);

                if(zit->second->getIsFiring()) {

                    //send an osc to supercollider
                    startSynth(zit->second);

                } else {

                    //send a stop osc
                }


            }

            ++zit;
        }
    }

    map<string, ofPtr<baseZode> > t_synths = pSplashManager->getSynths();
    map<string, ofPtr<baseZode> >::iterator sit = t_synths.begin();



    while(sit != t_synths.end()) {

        if(sit->second->getChanged() == CLAMOUR_ON_OFF) {
            sit->second->setChanged(CLAMOUR_NONE);

            if(sit->second->getIsFiring()) {

                //send an osc to supercollider
                startSynth(sit->second);
                if(sit->second->getEnvType() == CLAMOUR_AR) {
                    sit->second->setIsFiring(false);
                }

            } else {
                stopSynth(sit->second);
                sit->second->setIsFiring(false);
                //send a stop osc
            }


        }

        ++sit;
    }

}

void oscManager::sendBundle() {

    //send the client's outbundle to Meteor if it has messages waiting

    map<string, ofxOscBundle>::iterator osc_it;

    for(osc_it = outBundle.begin(); osc_it != outBundle.end(); osc_it++) {

        if(osc_it->second.getMessageCount() > 0) {
            sender.sendBundle(osc_it->second);
            osc_it->second.clear();
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

        addToBundle(clients[i], m);

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

        addToBundle(clients[i], m);

    }



}

void oscManager::addToBundle(string index, ofxOscMessage m) {

    if(outBundle.find(index) != outBundle.end()) {
        outBundle[index].addMessage(m);
    } else {
        ofxOscBundle b;
        outBundle[index] = b;
        outBundle[index].addMessage(m);
    }

}





//------------------------------SuperCollider Messages---------------------------------//

void oscManager::sendInit() {

    ofxOscMessage m;
    m.setAddress("/init");
    SCsender.sendMessage(m);

    logMessages(m, CLAMOUR_MSG_SC_OUT);

}

void oscManager::startSynth(ofPtr<baseZode> n) {

    baseData sd = n->getSoundData();
    if(sd.getName() == "none")return;

    ofxOscMessage m;
    m.setAddress("/startSynth");
    m.addStringArg(n->getName());

    m.addStringArg(sd.getName());

    string et = (n->getEnvType() == CLAMOUR_ASR)? "ASR" : "AR"; //might change to string later
    m.addStringArg(et);
    m.addFloatArg(n->getAttSecs());
    m.addFloatArg(n->getDecSecs());


    vector<float> vals = sd.getAbsVals();

    for(int i = 0; i < vals.size(); i++) {
        m.addFloatArg(vals[i]);
    }

    m.addStringArg(sd.getSoundFile());

    SCsender.sendMessage(m);
    logMessages(m, CLAMOUR_MSG_SC_OUT);

}



void oscManager::updateSynth(ofPtr<baseZode> n) {

    baseData sd = n->getSoundData();
    if(sd.getName() == "none")return;

    ofxOscMessage m;
    m.setAddress("/updateSynth");
    m.addStringArg(n->getName());



    m.addStringArg(sd.getName()); //add the name to keep the indexing th same for update and start at the SC end

    vector<float> vals = sd.getAbsVals();

    for(int i = 0; i < vals.size(); i++) {
        m.addFloatArg(vals[i]);
    }

    SCsender.sendMessage(m);

    logMessages(m, CLAMOUR_MSG_SC_OUT);


}

void oscManager::stopSynth(ofPtr<baseZode> n) {

    baseData sd = n->getSoundData();
    if(sd.getName() == "none")return;

    ofxOscMessage m;
    m.setAddress("/stopSynth");
    m.addStringArg(n->getName());
    SCsender.sendMessage(m);

    logMessages(m, CLAMOUR_MSG_SC_OUT);

}
