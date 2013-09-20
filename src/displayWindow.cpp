#include "displayWindow.h"



void displayWindow::setup()
{

    setWindowShape(200,200);
    setWindowPosition(0, 0);
    setWindowTitle("CLAMOUR DISPLAY");
    smallFont.loadFont("fonts/Goulong/Goulong.ttf", 10);


}

void displayWindow::update()
{


    //could do with a flag but leave for now
    mNodeRenderer.setScreen(getWidth(), getHeight());
    mTitleRenderer.setScreen(getWidth(), getHeight());

    //for sanity's sake this could go back to testApp
    pNodeManager->setScreenProp((float)getHeight()/getWidth());

    mTitleRenderer.update();
    ofBackground(0);
}


void displayWindow::draw()
{

    ofSetColor(255);
    smallFont.drawString(ofToString(ofGetFrameRate(),2),20,20);

    //draw the zones first
   // pZoneManager->
    //pZoneRenderer->draw();



    mNodeRenderer.renderNodes(pNodeManager->getActiveNodes());
    mTitleRenderer.draw();

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

void displayWindow::setZoneRenderer(ofPtr<zoneRenderer> p)
{

    pZoneRenderer = p;
}

void displayWindow:: setZoneManager(ofPtr<zoneManager> p)
{
    pZoneManager = p;
}

void displayWindow::addTitle(string s, title t)
{

    mTitleRenderer.addTitle(s, t);
}

void displayWindow::endTitle(string s)
{

    mTitleRenderer.endTitle(s);
}

void displayWindow::mouseMoved(int x, int y )
{


}

void displayWindow::reset()
{

    mTitleRenderer.reset();
}


