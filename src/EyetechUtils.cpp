#include "EyetechUtils.h"


ofVec2f EyetechUtils::gazeToVec2f(ofVec2f v2f_calibrated_w_h, ofVec2f v2f_calibrated_gaze_precentage)
{
	/*
	This function computers the inch scaled coordinate pair for calibrated gaze.
	The v2f return value should be added to a wcs v3f defining the top left corner of the calibrated area's physical location
	*/

	float cW =  v2f_calibrated_w_h.x; //width <- inch value
	float cH =  v2f_calibrated_w_h.y; //height <- inch value
		
	float gX = v2f_calibrated_gaze_precentage.x; //[0-1] value <- % of calibrated area
	float gY = v2f_calibrated_gaze_precentage.y; //[0-1] value <- % calibrated area

	/*
	//DEBUG
	cout << "%x: "<< v2f_calibrated_gaze_precentage.x << "\n";
	cout << "%y: "<< v2f_calibrated_gaze_precentage.y << "\n";
	cout << "\n";
	*/

	//calc
	float x = cW*gX;
	float y = cH*gY;
	
	/*
	//DEBUG
	cout << "v.x: "<< x << "\n";
	cout << "v.y: "<< y << "\n";
	cout << "\n";
	*/

	return ofVec2f(x, y);
}

void EyetechUtils::drawTick(QLXYPairFloat _p, int _s, ofColor _c){
	ofSetColor(_c);
	ofSetLineWidth(4);
	ofLine(_p.x-_s,_p.y,_p.x+_s,_p.y);
	ofLine(_p.x,_p.y-_s,_p.x,_p.y+_s);
	ofSetColor(255);
}




