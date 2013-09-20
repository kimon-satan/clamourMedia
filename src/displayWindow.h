#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

#include "nodeRenderer.h"
#include "zoneManager.h"
#include "zoneRenderer.h"
#include "titleRenderer.h"

#include "ofxFensterManager.h"
 #include "nodeManager.h"


class displayWindow : public ofxFenster{

    public:
        void setup();
        void update();
        void draw();

        void setNodeManager(ofPtr<nodeManager> p);
        void setZoneRenderer(ofPtr<zoneRenderer> p);
        void setZoneManager(ofPtr<zoneManager> p);

        void keyPressed(int key);
        void mouseMoved(int x, int y );

        void addTitle(string s, title t);
        void endTitle(string s);

        void reset();



    private:

     ofPtr<nodeManager> pNodeManager;
     ofPtr<zoneRenderer> pZoneRenderer;
     ofPtr<zoneManager> pZoneManager;

     nodeRenderer mNodeRenderer;
     titleRenderer mTitleRenderer;
     ofTrueTypeFont smallFont;




};

#endif // DISPLAYWINDOW_H
