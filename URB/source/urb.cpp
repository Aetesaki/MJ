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
//    PROTOCOL 2.3                     //
//    V.5.2019  Steve Massikker        //
//-------------------------------------//

#include "Arduino.h"
#include "urb.h"
#include <Wire.h>
#include <Servo.h>

const byte defaultSpeedArray[] = {20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,255};
const byte DIRECTION_FORWARD = 1;
const byte DIRECTION_BACKWARDS = 2;
void(* resetFunc) (void) = 0;

// arrays of L298 pins
int _L298EnablePin[10];
int _L298PinA[10];
int _L298PinB[10];

// temporary internal data variable
byte _dataReceivedFromBus;

// Internal function for receiving data from bus
void receiveFromBus(int howMany) {
	// While there are data available
	while (Wire.available() > 0) {
		// dump that data into the temporary data variable
		_dataReceivedFromBus = Wire.read();
	}
}

void urbclass::setup(void) {
	// Initializing bus as master
	Wire.begin();
	// Set PWM frequency for D9 & D10
	// Timer 1 divisor to 256 for PWM frequency of 122.55 Hz
	TCCR1B = TCCR1B & B11111000 | B00000100;   
}

void urbclass::setup(int address) {
	// Initializing bus as slave
	Wire.begin(address);
	// setting up bus interrupt to access internal function
	Wire.onReceive(receiveFromBus);
	// Set PWM frequency for D9 & D10
	// Timer 1 divisor to 256 for PWM frequency of 122.55 Hz
	TCCR1B = TCCR1B & B11111000 | B00000100;   
}

void urbclass::initializeTrack(byte track, int enablePin, int  pinA, int pinB) {
	// setting up track connection
	// use local variable, and subtract one from track to correspond with array
	byte local = track - 1;
	// save pins in internal array
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

// convert internal temporary data to readable data
byte urbclass::dataFromBus() {
	if (_dataReceivedFromBus) {
		return _dataReceivedFromBus;
		_dataReceivedFrom = 0;
}

// send data to bus
void urbclass::sendDataToBus(int address, byte data) {
	// Transmit to address
	Wire.beginTransmission(address);
	// Send data
	Wire.write(data);
	// Close transmission
	Wire.endTransmission();
}

// functions for controlling the tracks
void urbclass::controlTrack(byte track, byte speed) {
	// use local variable, and subtract one from track to correspond with array
	byte local = track - 1;
	// set the current PWM of track to speed
	analogWrite(_L298EnablePin[local], speed);
}

void urbclass::stopTrack(byte track) {
	// set the current PWM of track to 0
	controlTrack(track, 0);
}

void urbclass::reverseTrack(byte track, byte direction) {
	// first stop train
	stopTrack(track);
	// use local variable, and subtract one from track to correspond with array
	byte local = track - 1; 
	// Switch direction as required
	switch (direction) {
		case DIRECTION_FORWARD:
			digitalWrite(_L298PinA[local], HIGH);
			digitalWrite(_L298PinB[local], LOW);
			break;
		case DIRECTION_BACKWARDS:
			digitalWrite(_L298PinA[local], LOW);
			digitalWrite(_L298PinB[local], HIGH);
			break;
		default:
			break;
	}
}

urbClass URB;
