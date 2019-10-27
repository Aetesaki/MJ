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
#include <Wire.h>
#include <Servo.h>

const byte defaultSpeedArray[] = {20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,255};
void(* resetFunc) (void) = 0;
int _L298EnablePin[10];
int _L298PinA[10];
int _L298PinB[10];

void urbclass::setup(void) {
	// Initializing COMM
	Wire.begin();
	// Set PWM frequency for D9 & D10
	// Timer 1 divisor to 256 for PWM frequency of 122.55 Hz
	TCCR1B = TCCR1B & B11111000 | B00000100;   
}

void urbclass::setup(int address) {
	// Initializing COMM
	Wire.begin(address);
	// Set PWM frequency for D9 & D10
	// Timer 1 divisor to 256 for PWM frequency of 122.55 Hz
	TCCR1B = TCCR1B & B11111000 | B00000100;   
}

void urbclass::initializeTrack(byte track, int enablePin, int  pinA, int pinB) {
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
