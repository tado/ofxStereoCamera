#include "ofApp.h"

#include "ofxStereoCamera.h"

ofxStereoCamera<ofEasyCam> cam;

vector<ofVec3f> points;

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	
	ofBackground(0);
	
	cam.setup(ofGetWidth(), ofGetHeight());
	cam.setScale(1, -1, 1);
	
	cam.setPhysicalFocusDistance(120);
	
	cam.setFocusDistance(50);
	cam.setNearClip(0.1);
	
	float s = 1000;
	for (int i = 0; i < 3000; i++)
	{
		ofVec3f v;
		v.x = ofRandom(-s, s);
		v.y = ofRandom(-s, s);
		v.z = ofRandom(-s, s);
		points.push_back(v);
	}
}

//--------------------------------------------------------------
void ofApp::update()
{
	cam.setFocusDistance(cam.getGlobalPosition().length());
	
	cam.update();
	
	cam.beginLeft();
	drawScene();
	cam.endLeft();
	
	cam.beginRight();
	drawScene();
	cam.endRight();

}

void ofApp::drawScene()
{
	glPushMatrix();
	
	ofNoFill();
	ofBox(100);
	
	glEnable(GL_POINT_SMOOTH);
	
	ofDrawAxis(100);
	
	static GLfloat distance[] = { 0.0, 0.0, 1.0 };
	glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, distance);

	glPointSize(3000);
	
	glBegin(GL_POINTS);
	for (int i = 0; i < points.size();i ++)
	{
		glVertex3fv(points[i].getPtr());
	}
	glEnd();
	
	glPopMatrix();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	cam.draw(0, 0, ofGetWidth(), ofGetHeight());
	
	string str;
	str += "[1] SIDE_BY_SIDE\n";
	str += "[2] LINE_BY_LINE\n";
	str += "[3] ANAGLYPH\n";
	str += "[4] CROSS_EYED\n\n";
	str += "[5] PARALLEL\n";
	str += "[6] TOE_IN\n";
	
	ofSetColor(255);
	ofDrawBitmapString(str, 10, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
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

	if (key == 'f')
	{
		ofToggleFullscreen();
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