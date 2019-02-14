#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"

struct ofxStereoCameraViewMode
{
	enum Mode
	{
		PARALLEL,
		TOE_IN
	};
};

struct ofxStereoCameraOutputMode
{
	enum Mode
	{
		SIDE_BY_SIDE,
		CROSS_EYED,
		LINE_BY_LINE,
		SUTTER,
		ANAGLYPH,
		NO_STEREO
	};
};

/*
template <class OF_CAMERA_CLASS = ofEasyCam>
class ofxStereoCamera : public OF_CAMERA_CLASS
*/

class ofxStereoCamera : public ofEasyCam {

public:
	ofxStereoCamera() :
		physical_eye_seperation_cm(6.5),
		physical_focus_distance_cm(200),
		focus_distance(200),
		view(ofxStereoCameraViewMode::PARALLEL),
		output(ofxStereoCameraOutputMode::LINE_BY_LINE),
		ofEasyCam() {}

	void setup(int w, int h);
	void update(ofRectangle viewport = ofGetCurrentViewport());
	void draw(int x = 0, int y = 0);
	void draw(int x, int y, int w, int h);
	void setPhysicalEyeSeparation(float cm);
	void setPhysicalFocusDistance(float cm);
	void setFocusDistance(float v);
	void setViewMode(ofxStereoCameraViewMode::Mode m);
	void setOutputMode(ofxStereoCameraOutputMode::Mode m);

	void beginLeft();
	void endLeft();
	void beginRight();
	void endRight();
	ofTexture& getLeftTexture();
	ofTexture& getRightTexture();
	ofFbo * getLeftFbo();
	ofFbo * getRightFbo();
	void setupShader();
	void keyPressed(int key);
	void loadWarp();
	void saveWarp();
	void resetWarp();

	ofxStereoCameraViewMode::Mode view;
	ofxStereoCameraOutputMode::Mode output;

	int width, height;
	float eye, focus_distance;
	float physical_eye_seperation_cm;
	float physical_focus_distance_cm;
	ofFbo left_fbo, right_fbo;

	// toe-in
	float rot;

	// parallel
	float zNear, zFar;
	float fovx_tan, fovy_tan;
	float aspect;

	// sutter
	int frame_count;
	//void begin() {}
	//void end() {}
	ofShader shader;

	// Quad Warp
	glm::vec2 warpLeftTop;
	glm::vec2 warpRightTop;
	glm::vec2 warpLeftBottom;
	glm::vec2 warpRightBottom;
	int warpPoint;
	ofxXmlSettings xml;
};