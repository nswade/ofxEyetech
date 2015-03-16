#pragma once

#include "ofMain.h"
#include "QL2Utility.h"

class EyetechUtils 
{
	public :  
		ofVec2f gazeToVec2f(ofVec2f v2f_calibrated_w_h, ofVec2f v2f_calibrated_gaze_precentage);
		void drawTick(QLXYPairFloat _p, int _s, ofColor _c);
};