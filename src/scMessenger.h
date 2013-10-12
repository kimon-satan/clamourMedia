#ifndef SCMESSENGER_H
#define SCMESSENGER_H

#include "ofxOsc.h"
#include "clamourEvent.h"
#include "parameter.h"

#define SC_OUT_PORT 57120
#define NUM_MSG_STRINGS 6

class scMessenger
{
    public:
        scMessenger();
        virtual ~scMessenger();

            static void setup();
            static void sendInit();

            static void startSynth(string z_name, clamourEvent & e, baseData & sd);
            static void stopSynth(string z_name);
            static void stopSynth(string z_name, baseData & sd);
            static void updateSynth(string name, baseData & sd);

            static void logMessages(ofxOscMessage m);

            static string getMsgString();

            static ofxOscSender SCsender;
            static vector<string>  msg_strings;
            static int msg_string_count;

    protected:
    private:
};

#endif // SCMESSENGER_H
