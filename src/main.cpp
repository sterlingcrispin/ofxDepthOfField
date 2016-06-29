#include "ofMain.h"
#include "ofApp.h"
#include "ofGLProgrammableRenderer.h"

//========================================================================
int main( ){

    // check what version of OpenGL you have on your machine
    ofGLWindowSettings glWindowSettings;
    glWindowSettings.setGLVersion(4,1);
    ofCreateWindow(glWindowSettings);
    
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new ofApp());

}
