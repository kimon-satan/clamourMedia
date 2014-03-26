#include "displayWindow.h"



void displayWindow::setup()
{

    ofSetWindowShape(screenData::width, screenData::height);
    ofSetWindowPosition(50, 50);
    ofSetWindowTitle("CLAMOUR DISPLAY");
    smallFont.loadFont("fonts/Goulong/Goulong.ttf", 10);
    mZoneRenderer.setupPostProc();
    isScreenChanged = false;
    isFullScreen = false;

}

void displayWindow::update()
{

    //could do with a flag but leave for now
    if(isScreenChanged){
        screenData::width = ofGetWidth();
        screenData::height = ofGetHeight();
        mZoneRenderer.setupPostProc();
        pNodeManager->setupRegions();
        isScreenChanged = false;
    }

    ofBackground(0);
}


void displayWindow::draw()
{

    //draw the zones first

    mZoneRenderer.draw(pZoneManager->getZones());
    mNodeRenderer.renderNodes(pNodeManager->getNodes());
    mSplashRenderer.drawTitles(pSplashManager->getOnTitles());
    ofSetColor(255);
    smallFont.drawString(ofToString(ofGetFrameRate(),2),20,20);


}


void displayWindow::keyPressed(int key)
{

    //eventually move this to UI element
    if(key == 102)
    {
        isScreenChanged = true;
        isFullScreen = !isFullScreen;
        ofSetFullscreen(isFullScreen);
    }


}

void displayWindow::setNodeManager(ofPtr<nodeManager> p)
{

    pNodeManager = p;

}


void displayWindow:: setZoneManager(ofPtr<zoneManager> p)
{
    pZoneManager = p;
}

void displayWindow::setSplashManager(ofPtr<splashManager> p)
{
    pSplashManager = p;
}

void displayWindow::mouseMoved(int x, int y )
{


}




