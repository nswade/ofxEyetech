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



#include "ofxEyetech.h"

void ofxEyetech::init(string _dir, float calibrated_area_w, float calibrated_area_h)
{
	// Load the Quick Link 2 library. If if loaded successfully then print the library version.
	if(ql2Utility.Load()){ std::cout << "QuickLink2.dll loaded" << "\n";}else{ std::cout << "Could not load QuickLink2.dll" << "\n";}

	// Get the device ID for a device and initialize the device 
	//// with the settings in the specified settings file.
    deviceId = QL2Initialize("C:\\temp\\QL2Settings.txt");
	if(deviceId == 0){ std::cout << "Device ID not set" << "\n";}
	
	lEyeValid, rEyeValid, weightedValid = 0;

	dir =  _dir;
	v2f_calibrated_w_h.x = calibrated_area_w;
	v2f_calibrated_w_h.y = calibrated_area_h;

	ql_image_init = true; 
	qlGainMode = true; //auto
}


void ofxEyetech::start(){
	// Start the device.
	if(QLDevice_Start(deviceId) != QL_ERROR_OK){ std::cout << "Device Init Failed" << "\n";}

	// Display the live image.
	//DisplayExitCode displayExitCode = DisplayVideo(deviceId);

	QLCalibrationId calibrationId = 0;
	//Check for a saved calibration file
	ifstream file(dir.c_str());
	if (!file){
		// Calibrate the device.
		if(AutoCalibrate(deviceId, QL_CALIBRATION_TYPE_16, &calibrationId))
		{
			 QLDevice_ApplyCalibration(deviceId, calibrationId);

			//save the calibration
			if (QLCalibration_Save(dir.c_str(),calibrationId) == QL_ERROR_OK){
				std::cout << "Calibration data saved \n";
			}else{
				std::cout << "ERROR : Calibration data save failed \n";
			}
		}

	}else{
		//load calibration
		if (QLCalibration_Load(dir.c_str(),&calibrationId) == QL_ERROR_OK){
			QLDevice_ApplyCalibration(deviceId, calibrationId);
			std::cout << "Calibration data loaded \n";
		}else{
			std::cout << "ERROR : Calibration data load failed \n";
		}
	}
}


void ofxEyetech::stop(){
	//QLDevice_Stop_All();
}


void ofxEyetech::shutdown(){
	//ql2Utility.Unload();
}


void ofxEyetech::update(){
	QLFrameData frameData;
	QLError qlerror = QL_ERROR_OK;

	// Get a frame from the device. If there was an 
	// error getting the frame then return an error.
    if((qlerror = QLDevice_GetFrame(deviceId, 1000, &frameData)) != QL_ERROR_OK)
	{
		printf_s("Error getting frame from device. Error = %d\n", qlerror);     
		//return DEC_ERROR;
	}

	//\cv image stream
	if(ql_image_init){
		// do once
		// Create the OpenCV image objects and initialize the local pointers. 
		// The image from Quick Link 2 is 8 bit grey scale and the pixel data 
		// buffer is allocated in Quick Link 2 so only create an image header. 
		// The image that will be displayed has other colored things that are 
		// drawn on it so and its buffer is not created elsewhere so allocate 
		// three bytes per pixel.
		ql2Image = cvCreateImageHeader(cvSize(frameData.ImageData.Width, frameData.ImageData.Height), 8, 1);
		displayImage = cvCreateImage(cvSize(frameData.ImageData.Width, frameData.ImageData.Height), 8, 3);
		qlCvFbo.allocate(frameData.ImageData.Width, frameData.ImageData.Height, GL_RGB);
		ql_image_init = false;
	}
	// Assign the pixel data buffer pointer in the OpenCV image to the 
	// pixel data buffer in the Quick Link 2 frame data.
	ql2Image->imageData = (char*)frameData.ImageData.PixelData;
	// Copy the grey scale image to the color image buffer so it can be displayed.
    cvCvtColor(ql2Image, displayImage, CV_GRAY2RGB);

	//alolocate and copy rgb cv pixels into ofimage
	qlCvOfImage.allocate(frameData.ImageData.Width, frameData.ImageData.Height, OF_IMAGE_COLOR);
	qlCvOfImage.setFromPixels((unsigned char*)displayImage->imageData,frameData.ImageData.Width, frameData.ImageData.Height, OF_IMAGE_COLOR);
	qlCvOfImage.update();

	//\start fbo
	qlCvFbo.begin();
	//

	qlCvOfImage.draw(0,0,qlCvOfImage.width,qlCvOfImage.height);

	//\position is a percentage of the calibrated area
	if(frameData.WeightedGazePoint.Valid){
		v2f_calibrated_gaze_precentage_weighted.x = (float)frameData.WeightedGazePoint.x/100; 
		v2f_calibrated_gaze_precentage_weighted.y = (float)frameData.WeightedGazePoint.y/100;
		//DEBUG
		//cout << "Gaze X: " << (float)frameData.WeightedGazePoint.x << "\n";
		//cout << "Gaze Y: " << (float)frameData.WeightedGazePoint.y << "\n\n";
		weightedValid = 1;
	}else{weightedValid = 0;}


	if(frameData.LeftEye.Found){
		v2f_calibrated_gaze_precentage_lEye.x = (float)frameData.LeftEye.GazePoint.x/100;
		v2f_calibrated_gaze_precentage_lEye.y = (float)frameData.LeftEye.GazePoint.y/100;
		f_lEye_pupil_dia = frameData.LeftEye.PupilDiameter; 
		//DEBUG
		//cout << "lEye Gaze X: " << v2f_calibrated_gaze_precentage_lEye.x << "\n";
		//cout << "lEye Gaze Y: " << v2f_calibrated_gaze_precentage_lEye.y << "\n\n";
		lEyeValid = 1;

		//draw tick
		util.drawTick(frameData.LeftEye.Pupil, 20, ofColor(0,255,0));
		util.drawTick(frameData.LeftEye.Glint0, 20, ofColor(0,255,0));
		util.drawTick(frameData.LeftEye.Glint1, 20, ofColor(0,255,0));

	}else{lEyeValid = 0; f_lEye_pupil_dia = 0;}

	
	if(frameData.RightEye.Found){
		v2f_calibrated_gaze_precentage_rEye.x = (float)frameData.RightEye.GazePoint.x/100;
		v2f_calibrated_gaze_precentage_rEye.y = (float)frameData.RightEye.GazePoint.y/100;
		f_rEye_pupil_dia = frameData.RightEye.PupilDiameter; 
		/*
		//DEBUG
		cout << "rEye Gaze X: " << v2f_calibrated_gaze_precentage_rEye.x << "\n";
		cout << "rEye Gaze Y: " << v2f_calibrated_gaze_precentage_rEye.y << "\n\n";
		*/
		rEyeValid = 1;

		//draw tick
		util.drawTick(frameData.RightEye.Pupil, 20, ofColor(255,0,0));
		util.drawTick(frameData.RightEye.Glint0, 20, ofColor(255,0,0));
		util.drawTick(frameData.RightEye.Glint1, 20, ofColor(255,0,0));

	}else{rEyeValid = 0; f_rEye_pupil_dia = 0;}

	//\end fbo
	qlCvFbo.end();
	//

}


void ofxEyetech::draw(int _x, int _y, int _w, int _h){
	qlCvFbo.draw(_x,_y,_w,_h);
}

ofVec2f ofxEyetech::getCalibratedGazeInch_rEye(){
	return util.gazeToVec2f(v2f_calibrated_w_h, v2f_calibrated_gaze_precentage_rEye);
}

ofVec2f ofxEyetech::getCalibratedGazeInch_lEye(){
	return util.gazeToVec2f(v2f_calibrated_w_h, v2f_calibrated_gaze_precentage_lEye);
}

ofVec2f ofxEyetech::getCalibratedGazeInch_weighted(){
	return util.gazeToVec2f(v2f_calibrated_w_h, v2f_calibrated_gaze_precentage_weighted);
}

void ofxEyetech::setLight(bool _b){
	QLError qlerror = QL_ERROR_OK;
    QLDeviceInfo deviceInfo;
    QLSettingsId settingsId;

	qlerror = QLSettings_Create(0, &settingsId);

	if(_b){
		QLSettings_SetValueInt(settingsId, "DeviceIRLightMode", QL_DEVICE_IR_LIGHT_MODE_AUTO);
		cout<<"QL_DEVICE_IR_LIGHT_MODE_AUTO"<<"\n";

	}else{
		QLSettings_SetValueInt(settingsId, "DeviceIRLightMode", QL_DEVICE_IR_LIGHT_MODE_OFF);
		cout<<"QL_DEVICE_IR_LIGHT_MODE_OFF"<<"\n";	
	}
	QLDevice_ImportSettings(deviceId, settingsId);
	
}

void ofxEyetech::setExposure(int _i){
	/*
	#define QL_SETTING_DEVICE_EXPOSURE   "DeviceExposure"
	The exposure time in milliseconds for each frame. Possible values range from 1-50 ms.
	*/

	QLError qlerror = QL_ERROR_OK;
    QLDeviceInfo deviceInfo;
    QLSettingsId settingsId;

	qlerror = QLSettings_Create(0, &settingsId);

	QLSettings_SetValueInt(settingsId, "DeviceExposure", _i);
	cout<<"QL_SETTING_DEVICE_EXPOSURE set to "<<_i<<"\n";
}

void ofxEyetech::setGainMode(bool _b){

	/*
	#define QL_SETTING_DEVICE_GAIN_MODE   "DeviceGainMode"
	enum QLDeviceGainMode:
    QL_DEVICE_GAIN_MODE_AUTO : The device will automatically adjust the vain value.
    QL_DEVICE_GAIN_MODE_MANUAL : The device will use a fixed gain value. 
	*/
	QLError qlerror = QL_ERROR_OK;
    QLDeviceInfo deviceInfo;
    QLSettingsId settingsId;

	qlerror = QLSettings_Create(0, &settingsId);

	if(_b){
		QLSettings_SetValueInt(settingsId, "DeviceGainMode", QL_DEVICE_GAIN_MODE_AUTO);
		cout<<"QL_DEVICE_GAIN_MODE_AUTO"<<"\n";
		qlGainMode = QL_DEVICE_GAIN_MODE_AUTO;
	}else{
		QLSettings_SetValueInt(settingsId, "DeviceGainMode", QL_DEVICE_GAIN_MODE_MANUAL);
		cout<<"QL_DEVICE_GAIN_MODE_MANUAL"<<"\n";
		qlGainMode =  QL_DEVICE_GAIN_MODE_MANUAL;
	}
	QLDevice_ImportSettings(deviceId, settingsId);

}

void ofxEyetech::setGainVal(int _i){
	/*
	#define QL_SETTING_DEVICE_GAIN_VALUE   "DeviceGainValue"
	The gain value the device will use when the setting QL_SETTING_DEVICE_GAIN_MODE is set to QL_DEVICE_GAIN_MODE_MANUAL. 
	*/
	if(qlGainMode =  QL_DEVICE_GAIN_MODE_MANUAL){
		QLError qlerror = QL_ERROR_OK;
		QLDeviceInfo deviceInfo;
		QLSettingsId settingsId;

		qlerror = QLSettings_Create(0, &settingsId);
		
		QLSettings_SetValueInt(settingsId, "DeviceGainValue", _i);
		
		QLDevice_ImportSettings(deviceId, settingsId);

		//cout<<"QL_DEVICE_GAIN set to "<<_i<<"\n";
	}
}