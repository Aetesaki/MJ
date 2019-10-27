//-------------------------------------//
//    WWW.ARDUINORAILWAYCONTROL.COM    //
//-------------------------------------//
//    urb.cpp                          //
//    urb library                      //
//    URB 2 FINAL                      //
//    PROTOCOL 2.5                     //
//    PWM 122 Hz                       //
//    V.10.2019  Aetesaki              //
//                                     //
//    PROTOCOL 2.5                     //
//    V.5.2019  Steve Massikker        //
//-------------------------------------//

#include "Arduino.h"
#include "urb.h"
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Servo.h>

void(* resetFunc) (void) = 0;
string _inputString = "";
int _L298EnablePin[10];
int _L298PinA[10];
int _L298PinB[10];

void urbclass::setupComm(void) {
	// Initializing COMM
	Wire.begin();
	Serial.begin(9600);
	_inputString.reserve(4);
}

void urbclass::setupComm(int address) {
	// Initializing COMM
	Wire.begin(address);
	Serial.begin(9600);
	_inputString.reserve(4);
}

void urbclass::setupTrack(byte track, int enablePin, int  pinA, int pinB) {
	// setting up track connection
	byte local = track - 1;
	// save pins internally
	_L298EnablePin[local] = enablePin;
	_L298PinA[local] = pinA;
	_L298PinB[local] = pinB;
	// setting pins as OUTPUT
	pinMode(_L298EnablePin[local], OUTPUT);
	pinMode(_L298PinA[local], OUTPUT);
	pinMode(_L298PinB[local], OUTPUT);
	// enabling defaults
	digitalWrite(_L298PinA[local], HIGH);
	digitalWrite(_L298PinB[local], LOW);
}

urbClass URB;
