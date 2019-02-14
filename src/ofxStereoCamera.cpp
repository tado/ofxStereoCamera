#include "ofxStereoCamera.h"

void ofxStereoCamera::setup(int w, int h) {
	width = w;
	height = h;
	left_fbo.allocate(width, height, GL_RGB);
	right_fbo.allocate(width, height, GL_RGB);
	setupShader();
	resetWarp();
	loadWarp();
}

void ofxStereoCamera::update(ofRectangle viewport) {
	ofEasyCam::begin();
	ofEasyCam::end();

	eye = physical_eye_seperation_cm * 0.5 / physical_focus_distance_cm;

	aspect = viewport.width / viewport.height;
	fovy_tan = tanf(PI * this->getFov() / 360.);
	fovx_tan = fovy_tan * aspect;

	zNear = this->getNearClip();
	zFar = this->getFarClip();
}

void ofxStereoCamera::draw(int x, int y) {
	draw(x, y, width, height);
}

void ofxStereoCamera::draw(int x, int y, int w, int h) {
	ofPushStyle();
	ofPushMatrix();

	ofSetColor(255, 255);

	ofTranslate(x, y, 0);

	if (output == ofxStereoCameraOutputMode::SIDE_BY_SIDE) {
		right_fbo.draw(0, 0, w / 2, h);
		left_fbo.draw(0 + w / 2, 0, w / 2, h);
	}
	else if (output == ofxStereoCameraOutputMode::CROSS_EYED) {
		left_fbo.draw(w / 4, h / 4, w / 4, h / 4);
		right_fbo.draw(w / 2, h / 4, w / 4, h / 4);
		ofDrawCircle(w / 4 + w / 8, h / 2 + 20, 10);
		ofDrawCircle(w / 2 + w / 8, h / 2 + 20, 10);
	}
	else if (output == ofxStereoCameraOutputMode::LINE_BY_LINE) {
		shader.begin();
		shader.setUniformTexture("left", left_fbo.getTextureReference(), 1);
		shader.setUniformTexture("right", right_fbo.getTextureReference(), 2);

		glBegin(GL_TRIANGLE_STRIP);

		glTexCoord2f(0, 0);
		glVertex2f(warpLeftTop.x, warpLeftTop.y);

		glTexCoord2f(width, 0);
		glVertex2f(warpRightTop.x, warpRightTop.y);

		glTexCoord2f(0, height);
		glVertex2f(warpLeftBottom.x, warpLeftBottom.y);

		glTexCoord2f(width, height);
		glVertex2f(warpRightBottom.x, warpRightBottom.y);

		glEnd();

		shader.end();
	}
	else if (output == ofxStereoCameraOutputMode::ANAGLYPH) {
		ofPushStyle();

		ofEnableBlendMode(OF_BLENDMODE_ADD);

		ofSetColor(0, 255, 255);
		left_fbo.draw(0, 0, w, h);

		ofSetColor(255, 0, 0);
		right_fbo.draw(0, 0, w, h);

		ofPopStyle();
	}
	else if (output == ofxStereoCameraOutputMode::SUTTER) {
		frame_count++;
		frame_count = frame_count % 2;

		ofEnableBlendMode(OF_BLENDMODE_ALPHA);

		ofSetColor(255);
		if (frame_count > 0) {
			left_fbo.draw(0, 0, w, h);
		}
		else {
			right_fbo.draw(0, 0, w, h);
		}
	}
	else if (output == ofxStereoCameraOutputMode::NO_STEREO) {
		left_fbo.draw(0, 0);
	}

	ofPopMatrix();
	ofPopStyle();
}

void ofxStereoCamera::setPhysicalEyeSeparation(float cm) {
	physical_eye_seperation_cm = cm;
}
void ofxStereoCamera::setPhysicalFocusDistance(float cm) {
	physical_focus_distance_cm = cm;
}

void ofxStereoCamera::setFocusDistance(float v) {
	focus_distance = v;
}

void ofxStereoCamera::setViewMode(ofxStereoCameraViewMode::Mode m) {
	view = m;
}
void ofxStereoCamera::setOutputMode(ofxStereoCameraOutputMode::Mode m) {
	output = m;
}

void ofxStereoCamera::beginLeft() {
	left_fbo.begin();

	ofFloatColor c = ofGetCurrentRenderer()->getBackgroundColor();
	ofClear(c);

	ofPushView();
	ofPushMatrix();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (view == ofxStereoCameraViewMode::PARALLEL) {
		glFrustum(-zNear * (fovx_tan + eye), zNear * (fovx_tan - eye), -zNear * fovy_tan, zNear * fovy_tan, zNear, zFar);
		glTranslated(-eye * focus_distance, 0, 0);
	}
	else if (view == ofxStereoCameraViewMode::TOE_IN) {
		ofMultMatrix(this->getProjectionMatrix());
		glRotatef(-rot, 0, 1, 0);
		glTranslated(-eye * focus_distance, 0, 0);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	ofMultMatrix(this->getModelViewMatrix());
}

void ofxStereoCamera::endLeft() {
	ofPopMatrix();
	ofPopView();

	left_fbo.end();
}

void ofxStereoCamera::beginRight() {
	right_fbo.begin();

	ofFloatColor c = ofGetCurrentRenderer()->getBackgroundColor();
	ofClear(c);

	ofPushView();
	ofPushMatrix();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (view == ofxStereoCameraViewMode::PARALLEL) {
		glFrustum(-zNear * (fovx_tan - eye), zNear * (fovx_tan + eye), -zNear * fovy_tan, zNear * fovy_tan, zNear, zFar);
		glTranslated(eye * focus_distance, 0, 0);
	}
	else if (view == ofxStereoCameraViewMode::TOE_IN) {
		ofMultMatrix(this->getProjectionMatrix());
		glRotatef(rot, 0, 1, 0);
		glTranslated(eye * focus_distance, 0, 0);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	ofMultMatrix(this->getModelViewMatrix());
}

void ofxStereoCamera::endRight() {
	ofPopMatrix();
	ofPopView();

	right_fbo.end();
}

ofTexture& ofxStereoCamera::getLeftTexture() {
	return left_fbo.getTextureReference();
}

ofTexture& ofxStereoCamera::getRightTexture() {
	return right_fbo.getTextureReference();
}

ofFbo * ofxStereoCamera::getLeftFbo() {
	return &left_fbo;
}

ofFbo * ofxStereoCamera::getRightFbo() {
	return &right_fbo;
}

void ofxStereoCamera::setupShader() {
#define _S(code) # code
	const char *vs = _S
	(
		varying vec2 pos;

	void main()
	{
		gl_FrontColor = gl_Color;
		gl_TexCoord[0] = gl_MultiTexCoord0;
		gl_Position = ftransform();

		pos = gl_Vertex.xy;
	}
	);

	const char *fs = _S
	(
		uniform sampler2DRect left;
	uniform sampler2DRect right;

	varying vec2 pos;

	void main()
	{
		vec2 coord = gl_TexCoord[0].xy;
		bool odd = int(mod(pos.y, 2.0)) == 0;

		if (odd)
		{
			gl_FragColor = gl_Color * texture2DRect(left, coord);
		}
		else
		{
			gl_FragColor = gl_Color * texture2DRect(right, coord);
		}
	}
	);
#undef _S

	shader.setupShaderFromSource(GL_VERTEX_SHADER, vs);
	shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fs);
	shader.linkProgram();
}

void ofxStereoCamera::loadWarp() {
	xml.loadFile("warpSettings.xml");
	warpLeftTop = glm::vec2(xml.getValue("warp::lefttop::x", 0), xml.getValue("warp::lefttop::y", 0));
	warpRightTop = glm::vec2(xml.getValue("warp::righttop::x", ofGetWidth()), xml.getValue("warp::righttop::y", 0));
	warpLeftBottom = glm::vec2(xml.getValue("warp::leftbottom::x", 0), xml.getValue("warp::leftbottom::y", ofGetHeight()));
	warpRightBottom = glm::vec2(xml.getValue("warp::rightbottom::x", ofGetWidth()), xml.getValue("warp::rightbottom::y", ofGetHeight()));
}

void ofxStereoCamera::saveWarp() {
	xml.setValue("warp::lefttop::x", warpLeftTop.x);
	xml.setValue("warp::lefttop::y", warpLeftTop.y);
	xml.setValue("warp::righttop::x", warpRightTop.x);
	xml.setValue("warp::lefttop::y", warpRightTop.y);
	xml.setValue("warp::leftbottom::x", warpLeftBottom.x);
	xml.setValue("warp::leftbottom::y", warpLeftBottom.y);
	xml.setValue("warp::rightbottom::x", warpRightBottom.x);
	xml.setValue("warp::leftbottom::y", warpRightBottom.y);
	xml.saveFile("warpSettings.xml");
}

void ofxStereoCamera::resetWarp() {
	warpLeftTop = glm::vec2(0, 0);
	warpRightTop = glm::vec2(ofGetWidth(), 0);
	warpLeftBottom = glm::vec2(0, ofGetHeight());
	warpRightBottom = glm::vec2(ofGetWidth(), ofGetHeight());
	warpPoint = 0;
}

void ofxStereoCamera::keyPressed(int key) {
	if (key == OF_KEY_LEFT) {
		switch (warpPoint) {
		case 0:
			warpLeftTop.x--;
			break;
		case 1:
			warpRightTop.x--;
			break;
		case 2:
			warpLeftBottom.x--;
			break;
		case 3:
			warpRightBottom.x--;
			break;
		}
	}
	else if (key == OF_KEY_RIGHT) {
		switch (warpPoint) {
		case 0:
			warpLeftTop.x++;
			break;
		case 1:
			warpRightTop.x++;
			break;
		case 2:
			warpLeftBottom.x++;
			break;
		case 3:
			warpRightBottom.x++;
			break;
		}
	}
	else if (key == OF_KEY_UP) {
		switch (warpPoint) {
		case 0:
			warpLeftTop.y--;
			break;
		case 1:
			warpRightTop.y--;
			break;
		case 2:
			warpLeftBottom.y--;
			break;
		case 3:
			warpRightBottom.y--;
			break;
		}
	}
	else if (key == OF_KEY_DOWN) {
		switch (warpPoint) {
		case 0:
			warpLeftTop.y++;
			break;
		case 1:
			warpRightTop.y++;
			break;
		case 2:
			warpLeftBottom.y++;
			break;
		case 3:
			warpRightBottom.y++;
			break;
		}
	}
	else if (key == 'q') {
		warpPoint = 0;
	}
	else if (key == 'w') {
		warpPoint = 1;
	}
	else if (key == 'e') {
		warpPoint = 2;
	}
	else if (key == 'r') {
		warpPoint = 3;
	}
	else if (key == 's') {
		saveWarp();
	}
	else if (key == 'a') {
		resetWarp();
	}
}