#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	eyetech.init("C:\\CODE\\VISUAL_STUDIO\\_GITHUB_RELEASE\\addons\\ofxEyetech\\calibration\\calibration", 60, 30); //absolute file path, calibrated width height are in real world inch units
	eyetech.start();
	cout << "eyetech gaze sensor configured" << "\n";

}

//--------------------------------------------------------------
void testApp::update(){
	eyetech.update();

}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(50,50,50);
	ofSetColor(255);
	
	int h = 300;
	int lSize = 10;

	eyetech.draw(0,0,500,h);

    if(eyetech.lEyeValid){
		ofVec2f v = eyetech.getCalibratedGazeInch_lEye();
		std::stringstream ss;
		ss << "lEye x:" << v.x << " y:" <<v.y;
		ofDrawBitmapString(ss.str(), 0, h);
	}else{
		ofDrawBitmapString("lEye not vaild", 0, h);
	}

	if(eyetech.rEyeValid){
		ofVec2f v = eyetech.getCalibratedGazeInch_rEye();
		std::stringstream ss;
		ss << "rEye x:" << v.x << " y:" <<v.y;
		ofDrawBitmapString(ss.str(), 0, h-lSize);
	}else{
		ofDrawBitmapString("rEye not vaild", 0, h-lSize);
	}

	if(eyetech.weightedValid){
		ofVec2f v = eyetech.getCalibratedGazeInch_weighted();
		std::stringstream ss;
		ss << "weighted x:" << v.x << " y:" <<v.y;
		ofDrawBitmapString(ss.str(), 0, h-lSize-lSize);
	}else{
		ofDrawBitmapString("weighted not vaild", 0, h-lSize-lSize);
	}

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
