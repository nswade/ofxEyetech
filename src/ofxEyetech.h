//\ Eyetech QuickLINK 2.5.1 CPP x32 Interface
//\ rev.121813 Nathan Wade

//ofxEyetech NOTES
/*
OF header edited
of_v0.8.0_vs_release\libs\quicktime\include\processes.h(195)

#ifndef _WINBASE_
EXTERN_API( OSErr )
GetProcessInformation           (const ProcessSerialNumber * PSN, ProcessInfoRec *info)     THREEWORDINLINE(0x3F3C, 0x003A, 0xA88F);
#endif
*/

//#include <iostream> 
#include <stdio.h>
#include <conio.h>
#include "Initialize.h"
#include "Calibrate.h"
#include "opencvUtility.h"
#include <opencv2/imgproc/imgproc_c.h>

//#include "QLTypes.h"
//#include "QuickLink2.h"

#include "ofMain.h"
#include "EyetechUtils.h"

class ofxEyetech{
	private:
		QL2Utility ql2Utility;
		QLDeviceId deviceId;

	public:
		//path to calibration file
		string dir;

		EyetechUtils util;
				
		ofVec2f	v2f_calibrated_gaze_precentage_lEye, v2f_calibrated_gaze_precentage_rEye, v2f_calibrated_gaze_precentage_weighted;
		ofVec2f v2f_calibrated_w_h;

		float f_lEye_pupil_dia, f_rEye_pupil_dia;

		int lEyeValid, rEyeValid, weightedValid;

		void init(string _dir, float calibrated_area_w, float calibrated_area_h);
		void start();
		void stop();
		void shutdown();
		void update();
		void draw(int _x, int _y, int _w, int _h);
		ofVec2f getCalibratedGazeInch_rEye();
		ofVec2f getCalibratedGazeInch_lEye();
		ofVec2f getCalibratedGazeInch_weighted();
		void setLight(bool _b);
		void setExposure(int _i);
		void setGainMode(bool _b);
		void setGainVal(int _i);
				
		//cv image pointers
		IplImage* ql2Image;
		IplImage* displayImage;

		ofImage qlCvOfImage;

		bool ql_image_init;
		ofImage qlCvImage;
		ofFbo qlCvFbo;

		//#define QL_SETTING_DEVICE_DISTANCE   "DeviceDistance"
		//The approximate distance in centimeters from the user to the device.
		int qlDist, qlGainMode;

};