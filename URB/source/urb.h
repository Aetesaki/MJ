//-------------------------------------//
//    WWW.ARDUINORAILWAYCONTROL.COM    //
//-------------------------------------//
//    urb.h                            //
//    urb library header               //
//    URB 2 FINAL                      //
//    PROTOCOL 2.5                     //
//    PWM 122 Hz                       //
//    V.10.2019  Aetesaki              //
//                                     //
//    PROTOCOL 2.3                     //
//    V.5.2019  Steve Massikker        //
//-------------------------------------//

#ifndef urb_h
#define urb_h

#include "Arduino.h"

class urbClass {
	public:
		// public functions and variables
		void setupComm(void);
		void setupComm(int);
		void initializeTrack(byte,int,int,int);
		void sendDataToBus(int, byte);
		void controlTrack(byte, byte);
		void stopTrack(byte);
		void reverseTrack(byte, byte);
		byte defaultSpeedArray[];
		byte dataFromBus();
		byte DIRECTION_FORWARD;
		byte DIRECTION_BACKWARDS;
	private:
		// private functions and variables
		int _L298EnablePin[10];
		int _L298PinA[10];
		int _L298PinB[10];
		byte _dataReceivedFromBus;
};

extern urbClass URB;

#endif
