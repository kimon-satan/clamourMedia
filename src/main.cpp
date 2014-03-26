


#include "ofApp.h"
#include "ofAppGlutWindow.h"
#include "ofxFensterManager.h"

//--------------------------------------------------------------
int main(){
    
    ofSetupOpenGL(ofxFensterManager::get(), 450, 600, OF_WINDOW);
	ofRunFensterApp(new ofApp()); // start the app
}
