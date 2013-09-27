#include "displayWindow.h"



void displayWindow::setup()
{

    setWindowShape(screenData::width, screenData::height);
    setWindowPosition(50, 50);
    setWindowTitle("CLAMOUR DISPLAY");
    smallFont.loadFont("fonts/Goulong/Goulong.ttf", 10);

}

void displayWindow::update()
{

    //could do with a flag but leave for now
    screenData::width = getWidth();
    screenData::height = getHeight();

    ofBackground(0);
}


void displayWindow::draw()
{

    ofSetColor(255);
    smallFont.drawString(ofToString(ofGetFrameRate(),2),20,20);

    //draw the zones first

    mZoneRenderer.draw(pZoneManager->getZones());
    mNodeRenderer.renderNodes(pNodeManager->getNodes());
    mSplashRenderer.drawTitles(pSplashManager->getOnTitles());

}


void displayWindow::keyPressed(int key)
{

    //eventually move this to UI element
    if(key == 102)
    {

        toggleFullscreen();
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




