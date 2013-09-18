#include "ofMain.h"
#include "ofApp.h"
#include "ofxFensterManager.h"

//========================================================================
int main( ){

	ofxFensterManager::setup(450,600,OF_WINDOW);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
