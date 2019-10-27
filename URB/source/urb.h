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
//    PROTOCOL 2.5                     //
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
		void setupTrack(byte,int,int,int);
	private:
		// private functions and variables
		string _inputString;
		int _L298EnablePin[10];
		int _L298PinA[10];
		int _L298PinB[10];
};

extern urbClass URB;

#endif
