#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main() {

	ofSetupOpenGL(1920, 1080, OF_WINDOW);			// <-------- setup the GL context

	//8K 7680×4320
	//ofSetupOpenGL(7680, 4320, OF_WINDOW);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
