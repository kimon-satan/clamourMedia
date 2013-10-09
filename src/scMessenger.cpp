#include "scMessenger.h"

ofxOscSender scMessenger::SCsender;
vector<string> scMessenger::msg_strings;
int scMessenger::msg_string_count = 0;

scMessenger::scMessenger()
{
    //ctor
}

scMessenger::~scMessenger()
{
    //dtor
}


void scMessenger::setup(){

    SCsender.setup("localhost", SC_OUT_PORT);
    scMessenger::sendInit();

}


void scMessenger::startSynth(string z_name, clamourEvent & e, baseData & sd) {

    if(sd.getName() == "none")return;

    ofxOscMessage m;
    m.setAddress("/startSynth");
    m.addStringArg(z_name + "_" + ofToString(sd.getEventIndex(), 0));

    m.addStringArg(sd.getName());

    string et = (e.getEnvType() == CLAMOUR_ASR)? "ASR" : "AR"; //might change to string later
    m.addStringArg(et);
    m.addFloatArg(e.getAttSecs());
    m.addFloatArg(e.getDecSecs());


    vector<float> vals = sd.getAbsVals();

    for(int i = 0; i < vals.size(); i++) {
        m.addFloatArg(vals[i]);
    }

    m.addStringArg(sd.getSoundFile());

    SCsender.sendMessage(m);
    scMessenger::logMessages(m); //TODO can't log messages at the moment until OSCManager becomes singleton

}

void scMessenger::sendInit() {

    ofxOscMessage m;
    m.setAddress("/init");
    SCsender.sendMessage(m);

    scMessenger::logMessages(m);

}

void scMessenger::startSynth(ofPtr<baseZode> n) {

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
    scMessenger::logMessages(m);

}





void scMessenger::updateSynth(ofPtr<baseZode> n) {

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

    scMessenger::logMessages(m);

}

void scMessenger::stopSynth(ofPtr<baseZode> n) {

    baseData sd = n->getSoundData();

    ofxOscMessage m;
    m.setAddress("/stopSynth");
    m.addStringArg(n->getName());
    SCsender.sendMessage(m);

    scMessenger::logMessages(m);

}


void scMessenger::scMessenger::logMessages(ofxOscMessage m) {

    if(msg_string_count >= NUM_MSG_STRINGS)return; //for when there's loads of messages

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
    msg_strings.push_back( msg_string.substr(0,50)); //maximum 50 chars
    msg_string_count += 1;


    //make space for new messages
    if(msg_strings.size() > NUM_MSG_STRINGS)msg_strings.erase(msg_strings.begin());

}

string scMessenger::getMsgString(){

    string s;

    for(int i = 0; i < msg_strings.size(); i++) {

        s += msg_strings[i];
        s += "\n\n";

    }

    return s;

}
