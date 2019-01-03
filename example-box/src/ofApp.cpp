#include "ofApp.h"

void ofApp::setup() {
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(0);
	
	ofToggleFullscreen();	
	rotating = false;

	gui.setup();
	gui.add(physicalFocus.setup("Physical Focus Distance", 200, 0, 600));
	gui.add(focusDistance.setup("Focus Distance", 200, 0, 600));
	gui.add(eyeSeparation.setup("Eye Separation", 5, 0, 20));
	gui.loadFromFile("settings.xml");
	
	cam.setup(ofGetWidth(), ofGetHeight());
	cam.setScale(1, -1, 1);
	cam.setViewMode(ofxStereoCameraViewMode::PARALLEL);
	cam.setPhysicalEyeSeparation(4.0);

	boxes.assign(2000, ofBoxPrimitive());

	int s = 500;
	for (int i = 0; i < boxes.size(); i++) {
		ofBoxPrimitive& box = boxes[i];
		box.set(10);
		box.roll(ofRandom(0, 180));
		box.tilt(ofRandom(0, 180));
		float tx = ofRandom(-s, s);
		float ty = ofRandom(-s, s);
		float tz = ofRandom(-s, s);
		box.setPosition(tx, ty, tz);
		colors.push_back(ofColor(ofRandom(40, 55), ofRandom(100, 160), ofRandom(130, 220)));
	}
}

void ofApp::update() {
	cam.setPhysicalFocusDistance(physicalFocus);
	cam.setFocusDistance(focusDistance);
	cam.setPhysicalEyeSeparation(eyeSeparation);
	
	cam.update();
	
	cam.beginLeft();
	drawScene();
	cam.endLeft();
	
	cam.beginRight();
	drawScene();
	cam.endRight();
}

void ofApp::drawScene() {
	ofEnableLighting();
	ofEnableDepthTest();
	ofPushMatrix();
	light.setAreaLight(ofGetWidth(), ofGetWidth());
	light.setPosition(0, 0, 2000);
	light.setAmbientColor(ofFloatColor(0.5, 0.5, 0.5));
	light.setDiffuseColor(ofFloatColor(0.9, 0.9, 0.9));
	light.enable();

	if (rotating) {
		ofRotateXDeg(ofGetElapsedTimef() * 5);
		ofRotateYDeg(ofGetElapsedTimef() * 6);
		ofRotateZDeg(ofGetElapsedTimef() * 7);
	}
	for (int i = 0; i < boxes.size(); i++) {
		ofSetColor(colors[i]);
		boxes[i].draw();		
	}
	light.disable();
	ofPopMatrix();
	ofDisableDepthTest();
	ofDisableLighting();
}

//--------------------------------------------------------------
void ofApp::draw() {
	cam.draw(0, 0, ofGetWidth(), ofGetHeight());
	gui.draw();
	ofSetColor(255);
	ofDrawBitmapString(ofGetFrameRate(), 20, 200);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == '1') {
		cam.setOutputMode(ofxStereoCameraOutputMode::LINE_BY_LINE);
	}
	else if (key == '2') {
		cam.setOutputMode(ofxStereoCameraOutputMode::SUTTER);
	}
	else if (key == '3') {
		cam.setOutputMode(ofxStereoCameraOutputMode::ANAGLYPH);
	}
	else if (key == '4') {
		cam.setOutputMode(ofxStereoCameraOutputMode::CROSS_EYED);
	}
	else if (key == '5')	{
		cam.setViewMode(ofxStereoCameraViewMode::PARALLEL);
	}
	else if (key == '6'){
		cam.setViewMode(ofxStereoCameraViewMode::TOE_IN);
	}
	else if (key == 'r') {
		if (rotating == true) {
			rotating = false;
		}
		else {
			rotating = true;
		}
	}
	/*
	if (key == '1')
	{
		cam.setOutputMode(ofxStereoCameraOutputMode::SIDE_BY_SIDE);
	}
	else if (key == '2')
	{
		cam.setOutputMode(ofxStereoCameraOutputMode::LINE_BY_LINE);
	}
	else if (key == '3')
	{
		cam.setOutputMode(ofxStereoCameraOutputMode::ANAGLYPH);
	}
	else if (key == '4')
	{
		cam.setOutputMode(ofxStereoCameraOutputMode::CROSS_EYED);
	}
	
	if (key == '5')
	{
		cam.setViewMode(ofxStereoCameraViewMode::PARALLEL);
	}
	else if (key == '6')
	{
		cam.setViewMode(ofxStereoCameraViewMode::TOE_IN);
	}
	else if (key == '7')
	{
		cam.setOutputMode(ofxStereoCameraOutputMode::SUTTER);
	}
	*/

	if (key == 'f')
	{
		ofToggleFullscreen();
		ofSetWindowPosition(0, 0);
		ofSetWindowShape(3840, 2160);
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{

}