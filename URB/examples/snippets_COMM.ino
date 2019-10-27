//-------------------------------------//
//    WWW.ARDUINORAILWAYCONTROL.COM    //
//-------------------------------------//
//    Snippet_COMM.ino                 //
//    COMM UNIT                        //
//    URB 2 FINAL                      //
//    PROTOCOL 2.5                     //
//    PWM 122 Hz                       //
//    V.5.2019  Steve Massikker        //
//-------------------------------------//

void(* resetFunc) (void) = 0;

#include <SoftwareSerial.h>
#include <Wire.h>
#include "urb.h"

SoftwareSerial Bluetooth(12, 13); // RX, TX

// VARIABLES
  
// Variables for decoding commands from external sources
bool stringComplete = false; 
String inputString = "";

// Speed arrays
// Edit as you please, or use default speed array
byte speedArrayA [] = URB.defaultSpeedArray:
byte speedArrayB [] = {34,40,45,50,55,60,70,80,90,100,110,120,130,140,170,180,190,200,210,220,230,240,250,255};

// URB units array
// add address of URB units to this array
// Addresses doesn't have to be in numeric order
byte URBUnits [] = { 2, 3, 4, 5, 67, 103 };

// switches
bool switch_A = true, switch_B = true, switch_C = true, switch_D = true,
     switch_E = true, switch_F = true, switch_G = true, switch_H = true, 
     switch_I = true, switch_J = true, switch_K = true, switch_L = true,
     switch_M = true, switch_N = true, switch_O = true, switch_P = true;

// flags
bool flag_change_junc = false;    

// setup
void setup() {
// Initializing COMM
  Serial.begin(9600);
  Bluetooth.begin(9600);
  inputString.reserve(4);
  URB.setupComm();
  // Initializing Track (track number, enable pin, pinA, pinB)
  // NOTE: Tracknumber must be higher than 0
  URB.initializeTrack(1, 10, 6, 5);
  URB.initializeTrack(2, 9, 4, 3);
  // EXAMPLE other tracks can be controlled from other units
  // in this case track 9 and 10 
}

void loop() {
  // ----  START PARSING INCOMING APP COMMANDS
  if (stringComplete) {
    // RESET on receiving code 999z
    if (inputString =="999z") {
	  for (byte i = 0; i < sizeof(URBUnits) - 1; i++ ) {
		// send command 99 to each URB unit listed in the array
	    URB.sendDataToBus(URBUnits[i], 99);
	  }
	  // reset master
      resetFunc();
    }  

    // Further parsing
    // Edit as required
    if (inputString.charAt(0) =='a') decodeSpeedForTrack(1); //local track
    if (inputString.charAt(0) =='b') decodeSpeedForTrack(2); //local track
    if (inputString.charAt(0) =='i') decodeSpeedForTrack(9); //track elsewhere
    if (inputString.charAt(0) =='k') decodeSpeedForTrack(10);//track elsewhere
    if (inputString.charAt(0) =='j') controlJunctions();  
    if (inputString.charAt(0) =='l') controlLights();
    if (inputString.charAt(0) =='g') controlGears(); 
    // parsing complete
    inputString = "";
    stringComplete = false;
  }

  // ----  MAIN LOGIC BLOCK
  // Edit as required
  if (flag_change_junc) {

     // Algorithm

    flag_change_junc = false;
  }

  bluetoothEvent();
}

//// FUNCTIONS ////
void bluetoothEvent() {
  if (Bluetooth.available()) {
    char inChar = (char)Bluetooth.read();
    inputString += inChar;
    if (inChar == 'z') {
      stringComplete = true;
    }
  }
}

void serialEvent() {
  if (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == 'z') {
      stringComplete = true;
    }
  }
}
