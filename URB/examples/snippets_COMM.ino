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
  
bool stringComplete = false; 
String inputString = "";

byte speedArrayA [] = {20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,255};
byte speedArrayB [] = {20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,255};

bool switch_A = true, switch_B = true, switch_C = true, switch_D = true,
     switch_E = true, switch_F = true, switch_G = true, switch_H = true, 
     switch_I = true, switch_J = true, switch_K = true, switch_L = true,
     switch_M = true, switch_N = true, switch_O = true, switch_P = true;

bool flag_change_junc = false;    


void setup() {

// Initializing COMM
  Serial.begin(9600);
  Bluetooth.begin(9600);
  inputString.reserve(4);
  URB.setupComm();

// Initializing Motor-Driver
  URB.initializeTrack(1, 10, 6, 5);
  URB.initializeTrack(2, 9, 4, 3);

}

void loop() {
  
  // ----  START PARSING INCOMING APP COMMANDS
  if (stringComplete) {
    // RESET 
    if (inputString =="999z") {
	  URB.sendDataToBus(2, 99);
	  URB.sendDataToBus(3, 99);
	  URB.sendDataToBus(4, 99);
      resetFunc();
    }  

    // FUNCTIONS 
    if (inputString.charAt(0) =='a') controlPlayerA();
    if (inputString.charAt(0) =='b') controlPlayerB();
    if (inputString.charAt(0) =='j') controlJunctions();  
    if (inputString.charAt(0) =='l') controlLights();
    if (inputString.charAt(0) =='g') controlGears(); 

    inputString = "";
    stringComplete = false;
  
  }

  // ----  MAIN LOGIC BLOCK
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
