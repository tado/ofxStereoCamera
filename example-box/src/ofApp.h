#pragma once

#include "ofMain.h"
#include "ofxStereoCamera.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp {
public:
	
	void setup();
	void update();
	void draw();
	
	void drawScene();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	ofxPanel gui;
	ofxFloatSlider physicalFocus;
	ofxFloatSlider focusDistance;
	ofxFloatSlider eyeSeparation;

	vector< ofBoxPrimitive > boxes;
	vector< ofColor > colors;

	ofLight light;
	bool rotating;

	ofxStereoCamera cam;
	vector<ofVec3f> points;
};