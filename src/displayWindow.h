#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

#include "nodeRenderer.h"
#include "zoneRenderer.h"
#include "splashRenderer.h"

#include "ofxFensterManager.h"

#include "nodeManager.h"
#include "zoneManager.h"
#include "splashManager.h"


class displayWindow : public ofxFensterListener{

    public:
        void setup();
        void update();
        void draw();

        void setNodeManager(ofPtr<nodeManager> p);
        void setZoneManager(ofPtr<zoneManager> p);
        void setSplashManager(ofPtr<splashManager> p);

        void keyPressed(int key);
        void mouseMoved(int x, int y );



    private:

     ofPtr<nodeManager> pNodeManager;
     ofPtr<zoneRenderer> pZoneRenderer;
     ofPtr<zoneManager> pZoneManager;
     ofPtr<splashManager> pSplashManager;

     nodeRenderer mNodeRenderer;
     splashRenderer mSplashRenderer;
     zoneRenderer mZoneRenderer;

     ofTrueTypeFont smallFont;
    bool isScreenChanged, isFullScreen;
    


};

#endif // DISPLAYWINDOW_H
