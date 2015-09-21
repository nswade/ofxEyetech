#ofxEyetech

ofxEyetech provides a simple Quicklink API wrapper for Eyetech gaze sensor devices. Eyetech sensors use an infrared illuminated PTGrey camera to measure binocular gaze in a calibrated area. Angular saccade behaviour, fixation, binocular coordination and stimuli response can be measured easily with this device. It is also used as an HCI interface, allowing a user to control an operating system or drive input with intentional gaze.

The gaze area is captured interactively through calibrated gazing. Deleting addons/ofxEyetech/calibration/calibration will prompt the calibration window on start up and generate a new file.

The QuickLINK library uses a contained OpenCV2.2 framework and ofxOpenCV is not required to build the example.

Quicktime in the openframeworks build may conflict with the Quicklink libs. This header section starting on line 195 should be edited: 

of_v0.8.0_vs_release\libs\quicktime\include\processes.h

'''
#ifndef _WINBASE_
EXTERN_API( OSErr )
GetProcessInformation           (const ProcessSerialNumber * PSN, ProcessInfoRec *info)     THREEWORDINLINE(0x3F3C, 0x003A, 0xA88F);
#endif
'''
