#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

#include "nodeRenderer.h"
#include "titleRenderer.h"

#include "ofxFensterManager.h"
 #include "nodeManager.h"


class displayWindow : public ofxFenster{

    public:
        void setup();
        void update();
        void draw();

        void setNodeManager(ofPtr<nodeManager> p);
        void keyPressed(int key);
        void mouseMoved(int x, int y );

        void addTitle(string s, title t);
        void endTitle(string s);




    private:

     ofPtr<nodeManager> pNodeManager;
     nodeRenderer mNodeRenderer;
     titleRenderer mTitleRenderer;
     ofTrueTypeFont largeTitle;




};

#endif // DISPLAYWINDOW_H
