//-------------------------------------//
//    WWW.ARDUINORAILWAYCONTROL.COM    //
//-------------------------------------//
//    test-track_secondary_test        //
//                                     //
//    HARDWARE: URB 2 FINAL            //
//    PWM: 122 Hz                      //
//                                     //
//    URB PROTOCOL 2.5dev              //
//    12.2019  Aetesaki                //
//                                     //
//    URB PROTOCOL 2.3 by              //
//    V.5.2019  Steve Massikker        //
//-------------------------------------//
/* 
This sketch is made up of several sections

Some sections you have to edit for the URB to function correctly, 
whilst others are not ment to be edited.

The sections are as following:
--------------------------------------
  1. LAYOUT CONSTANTS
  2. LAYOUT ARRAYS
  3. INITIALIZATION FUNCTIONS
  4. TRACK CONTROL FUNCTIONS
  5. TURNOUT FUNCTIONS
  6. SIGNAL FUNCTIONS
  7. ACCESSSORY FUNCTIONS
  8. YOUR FUNCTIONS
  9. ARDUNIO SETUP FUNCTION
 10. ARDUINO LOOP FUNCTION
--------------------------------------
Each section includes a heading telling you whether or not this 
section can be edited,

--------------------------------------
  WARNING! WARNING! WARNING! WARNING!
--------------------------------------
Note that only pins D3, D9, D10, and D11 can be used as EnablePins 
for tracks. Use of any other pins WILL fail without warning

Note that whilst it is possible to power the URB using the 5V output 
of your L298 module, beware that this can only  be done if the supply 
voltage to the module is less than or equal to 12 Volt, and you only 
supply power to a single URB unit.
A separate power supply for the URB units ARE recomended.
*/

/*
--------------------------------------
  1. LAYOUT CONSTANTS
--------------------------------------

The following constants are used to define the section of track 
controlled by this URB

These constants MUST be set correctly!

Features not used by this URB must be commented out.

*/

// # Number of tracks controlled by this URB
int const _numTracks = 1; 
// # Number of turnouts/switches controlled by this URB
int const _numTurnouts = 1; 
// # Number of 3 way turnouts/switches controlled by URB
// int const _num3ways = 0;
// # Number of slips controlled by URB
//int const _numSlips = 0;
// # Number of stop sections controlled by URB
int const _numStopSections = 1;
// # Number of feedback positions controlled by this URB
// (a feedback position is any of the following features:
// contact track, circuit track, reed contact, or light beam)
int const _numFeedbacks = 2;

// End of layout constants

/*
--------------------------------------
  2. LAYOUT ARRAYS
--------------------------------------

This section contains a list of arrays used by the URB to access the 
features controlled by this URB.

DO NOT EDIT THIS SECTION!

DO COMMENT OUT FEATURES NOT PRESENT ON THIS URB

*/

// Default Speed Array (24 speed steps)
// Do not edit these constants, you will make your own later
byte const defaultSpeedArray[] = {20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,255};
//byte const sizeOf_defaultSpeedArray = byte(sizeof(defaultSpeedArray)/sizeof(defaultSpeedArray[0]));

// arrays of tracks
// COMMENT OUT IF NO TRACKS ARE CONTROLLED BY THIS URB
// arrays of L298 pins
int _L298EnablePin[_numTracks];
int _L298PinA[_numTracks];
int _L298PinB[_numTracks];


// arrays of turnout
// COMMENT OUT IF NO TURNOUTS ARE CONTROLLED BY THIS URB
// arrays of pins
int turnoutNormal[_numTurnouts];
int turnoutDiverging[_numTurnouts];
// Duration to set the turnout (in ms)
int turnoutDuration[_numTurnouts]; 
// test array to see if a turnout is in operation
bool turnoutActive[_numTurnouts]; 
// array of timestamps for last activation
unsigned long turnoutActivated[_numTurnouts]; 

/* 
// arrays of 3-way points
// COMMENT OUT IF NO 3-WAY TURNOUTS ARE CONTROLLED BY THIS URB
// arrays of pins
int turnout3Right[_num3ways];
int turnout3Normal[_num3ways];
int turnout3left[_num3ways];
// Duration to set the turnout (in ms)
int turnout3Duration[_num3ways]; 
// test array to see if a turnout is in operation
bool turnout3Active[_num3ways]; 
// array of timestamps for last activation
unsigned long turnout3Activated[_num3ways]; 
*/
/*
// arrays of slips
// COMMENT OUT IF NO SLIPS ARE CONTROLLED BY THIS URB
// arrays of pins
int slipNormal[_numSlips];
int slipDiverging[_numSlips];
// Duration to set the slip (in ms)
int slipDuration[_numSlips]; 
// test array to see if a turnout is in operation
bool slipActive[_numSlips]; 
// array of timestamps for last activation
unsigned long slipActivated[_numSlips]; 
*/

// arrays of stop sections
// COMMENT OUT IF NO STOP SECTIONS ARE CONTROLLED BY THIS URB
// arrays of pins
int stopSection[_numStopSections];
// Duration to set the stop section (in ms)
int stopSectionDuration[_numStopSections]; 
// test array to see if a stop section is in operation
bool stopSectionActive[_numStopSections]; 
// array of timestamps for last activation
unsigned long stopSectionActivated[_numStopSections]; 


// arrays of feedback positions
// COMMENT OUT IF NO FEEDBACK POSITIONS ARE CONTROLLED BY THIS URB
// arrays of pins
int feedbacks[_numFeedbacks];
// test array to see if a feedback position is in operation
bool feedbacksActive[_numFeedbacks]; 

// End of layout arrays

/*
--------------------------------------
  3. INITIALIZATION FUNCTIONS
--------------------------------------

This section contains a list of initialization functions used by the URB 
to access the features controlled by this URB.

DO NOT EDIT THIS SECTION!

DO COMMENT OUT FEATURES NOT PRESENT ON THIS URB
*/


// Track initialization function
// COMMENT OUT IF NO TRACKS ARE CONTROLLED BY THIS URB
// directional constants
byte const DIRECTION_FORWARD = 1;
byte const DIRECTION_BACKWARDS = 2;
bool port1FreqSet = false;
bool port2FreqSet = false;

void initializeTrack(byte track, int enablePin, int  pinA, int pinB) {
  // as a default, always fail
  bool failed=true;
  // check if pin is port1
  if (enablePin==9 || enablePin==10 ) {
    // check if port 1 frequency is not set
    if (!port1FreqSet) {
      // set PWM frequency of port 1 to 122Hz
      TCCR1B = TCCR1B & B11111000 | B00000100;
      // store this fact for the future
      port1FreqSet=true;
      // allow the track to be set
      failed=false;
    }
  }
  // check if pin is port2
  if (enablePin==3 || enablePin==11 ) {
    // check if port 2 frequency is not set
    if (!port2FreqSet) {
      // set PWM frequency of port 2 to 122Hz
      TCCR2B = TCCR2B & B11111000 | B00000110;
      // store this fact for the future
      port2FreqSet=true;
      // allow the track to be set
      failed=false;
    }
  }
  // if port tests didn't fail, store the data
  if (!failed) {
    // use local variable, and subtract one from track to correspond with array
    byte local = track - 1;
    // save pins in internal array
    _L298EnablePin[local] = enablePin;
    _L298PinA[local] = pinA;
    _L298PinB[local] = pinB;
    // set pins as OUTPUT
    pinMode(_L298EnablePin[local], OUTPUT);
    pinMode(_L298PinA[local], OUTPUT);
    pinMode(_L298PinB[local], OUTPUT);
    // Set direction to forward
    reverseTrack(track,DIRECTION_FORWARD);
  }
}


// Turnout initialization functions
// COMMENT OUT IF NO TURNOUTS ARE CONTROLLED BY THIS URB
void initializeTurnout(byte turnout, int normalPin, int divergingPin, int duration) {
  // use local variable, and subtract one from turnout to correspond with array
  byte local = turnout - 1;
  // save pins in internal array
  turnoutNormal[local]=normalPin;
  turnoutDiverging[local]=divergingPin;
  // save duration in internal array
  turnoutDuration[local]=duration;
  // set pins as OUTPUT
  pinMode(turnoutNormal[local], OUTPUT);
  pinMode(turnoutDiverging[local], OUTPUT);
  // testing turnout
  digitalWrite(turnoutDiverging[local],HIGH);
  delay(turnoutDuration[local]);
  digitalWrite(turnoutDiverging[local],LOW);
  digitalWrite(turnoutNormal[local],HIGH);
  delay(turnoutDuration[local]);
  digitalWrite(turnoutNormal[local],LOW);
}
/*
void initializeTurnout(byte turnout, int normalPin, int divergingPin) {
  // initializing turnout with a duration of 200ms
  initializeTurnout(turnout, normalPin, divergingPin, 200);
  //Serial.println(F("initialized turnout at 200ms"));
}
*/
/*
// 3-way turnout initialization functions
// COMMENT OUT IF NO 3-WAY TURNOUTS ARE CONTROLLED BY THIS URB
void initialize3Turnout(byte turnout, int normalPin, int rightPin, int leftPin, int duration) {
  // use local variable, and subtract one from turnout to correspond with array
  byte local = turnout - 1;
  // save pins in internal array
  turnout3Normal[local]=normalPin;
  turnout3Right[local]=rightPin;
  turnout3left[local]=leftPin;
  // save duration in internal array
  turnout3Duration[local]=duration;
  // set pins as OUTPUT
  pinMode(turnout3Normal[local], OUTPUT);
  pinMode(turnout3Right[local], OUTPUT);
  pinMode(turnout3left[local], OUTPUT);
  // testing turnout
  digitalWrite(turnout3left[local],HIGH);
  delay(turnout3Duration[local]);
  digitalWrite(turnout3left[local],LOW);
  digitalWrite(turnoutNormal[local],HIGH);
  delay(turnout3Duration[local]);
  digitalWrite(turnoutNormal[local],LOW);
  digitalWrite(turnout3Right[local],HIGH);
  delay(turnout3Duration[local]);
  digitalWrite(turnout3Right[local],LOW);
  digitalWrite(turnoutNormal[local],HIGH);
  delay(turnout3Duration[local]);
  digitalWrite(turnoutNormal[local],LOW);
}
*/
/*
void initialize3Turnout(byte turnout, int normalPin, int rightPin, int leftPin) {
  // initializing turnout with a duration of 200ms
  initialize3Turnout(turnout, normalPin, rightPin, leftPin, 200);
}
*/
/*
// Slip initialization functions
// COMMENT OUT IF NO SLIPS ARE CONTROLLED BY THIS URB
void initializeSlip(byte slip, int normalPin, int divergingPin, int duration) {
  // use local variable, and subtract one from slip to correspond with array
  byte local = slip - 1;
  // save pins in internal array
  slipNormal[local]=normalPin;
  slipDiverging[local]=divergingPin;
  // save duration in internal array
  slipDuration[local]=duration;
  // set pins as OUTPUT
  pinMode(slipNormal[local], OUTPUT);
  pinMode(slipDiverging[local], OUTPUT);
  // testing turnout
  digitalWrite(slipDiverging[local],HIGH);
  delay(slipDuration[local]);
  digitalWrite(slipDiverging[local],LOW);
  digitalWrite(slipNormal[local],HIGH);
  delay(slipDuration[local]);
  digitalWrite(slipNormal[local],LOW);
}
void initializeSlip(byte slip, int normalPin, int divergingPin) {
  // initializing slip with a duration of 200ms
  initializeSlip(slip, normalPin, divergingPin, 200);
}
*/

// Stop section initialization functions
// COMMENT OUT IF NO STOP SECTIONS ARE CONTROLLED BY THIS URB
void initializeStopSection(byte section, int outpin, int duration) {
  // use local variable, and subtract one from section to correspond with array
  byte local = section - 1;
  // save pin in internal array
  stopSection[local]=outpin;
  // save duration in internal array
  stopSectionDuration[local]=duration;
  // set pin as OUTPUT
  pinMode(stopSection[local], OUTPUT);
  // testing stop section
  digitalWrite(stopSection[local],HIGH);
  // if duration is 0
  if (duration==0) {
    // hold for 250 ms
    delay(250);
  } else {
    // hold for duration
    delay(stopSectionDuration[local]);
  }
  digitalWrite(stopSection[local],LOW);
}
/*
void initializeStopSection(byte section, int pin) {
  // initializing stop section with duration of 75ms
  //Serial.println(F("initilizing stop section at 75ms"));
  initializeStopSection(section, pin, 75);
}
*/

// Feedback initialization functions
// COMMENT OUT IF NO FEEDBACKS ARE USED BY THIS URB
void initializeFeedback(byte feedback, int pin, bool pullup) {
  // use local variable, and subtract one from feedback to correspond with array
  byte local = feedback - 1;
  // save pin in internal array
  feedbacks[local]=pin;
  // set pin as INPUT or INPUT_PULLUP
  if (pullup) {
    // set pin as INPUT_PULLUP
    pinMode(feedbacks[local],INPUT_PULLUP);
  } else {
    // set pin as INPUT
    pinMode(feedbacks[local],INPUT);
  }
}
/*
void initializeFeedback(byte feedback, int pin) {
  // initializing feedback with pullup set
  initializeFeedback(feedback, pin, true);
}
*/
// End of initialization functions

/*
--------------------------------------
  4. TRACK CONTROL FUNCTIONS
--------------------------------------

This section contains a list of track control functions used by this URB

You may add track control functions to this section

Comment out these functions if this URB doesn't control any tracks.

*/

void controlTrack(byte track, byte speed) {
  // use local variable, and subtract one from track to correspond with array
  byte local = track - 1;
  // set the current PWM of track to speed
  analogWrite(_L298EnablePin[local], speed);
}

void stopTrack(byte track) {
  // set the current PWM of track to 0
  controlTrack(track, 0);
}

void reverseTrack(byte track, byte direction) {
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

// End of track control functions

/*
--------------------------------------
  5. TURNOUT FUNCTIONS
--------------------------------------

This section contains a list of turnout functions, including 3 way turnouts and
slips, used by this URB

You may add your own functions to this section

Comment out this section if this URB doesn't controll any turnouts

*/

void setTurnout(int turnout, bool normal) {
  // use local variable, and subtract one from turnout to correspond with arrray
  byte local = turnout - 1;
  // set turnout
  if (normal) {
    // set turnout to normal
    digitalWrite(turnoutNormal[local],HIGH);
  } else {
    // set turnout to diverging
    digitalWrite(turnoutDiverging[local],HIGH);
  }
  // set turnout as activated
  turnoutActive[local]=true;
  // save timestamp
  turnoutActivated[local]=millis();
}

void unsetTurnout(int turnout) {
  // use local variable, and subtract one from turnout to correspond with arrray
  byte local = turnout - 1;
  // unset turnout
  digitalWrite(turnoutNormal[local],LOW);
  digitalWrite(turnoutDiverging[local],LOW);
  // set turnout as deactivated
  turnoutActive[local]=false;
  // save timestamp
  turnoutActivated[local]=0;  
}
bool isTurnoutActivated(int turnout) {
  // use local variable, and subtract one from turnout to correspond with arrray
  byte local = turnout - 1;
  // return the internal value
  return turnoutActive[local];
}
void iterateTurnouts() {
  // iterate through turnouts
  // Note that while the arrays starts at 0, all functions starts at 1
  // for each turnout
  for (int currentTurnout=0; currentTurnout<_numTurnouts; currentTurnout++) {
    // check if the turnout is activated
    if (isTurnoutActivated(currentTurnout+1)) {
      // if turnout is activated, check if current time is passed its duration time
      if (turnoutActivated[currentTurnout]  + turnoutDuration[currentTurnout] <= millis()) {
        // if so, unset the turnout
        unsetTurnout(currentTurnout+1);
      }
    }
  }
}

// End of turnout functions

/*
--------------------------------------
  6. SIGNAL FUNCTIONS
--------------------------------------

This section contains a list of signal functions used by this URB.

Add your own functions here

Include any stop section functions here

*/

void setStopSection(int section, bool high) {
  // use local variable, and subtract one from section to correspond with arrray
  byte local = section - 1;
  // set stop section
  if (high) {
    // set stop section to high
    digitalWrite(stopSection[local],HIGH);
    Serial.println(F("Stop section set high"));
  } else {
    // set stop section to low
    digitalWrite(stopSection[local],LOW);
  }
  // if stop section duration is set
  if (stopSectionDuration[local]>0) {
    // set stop section as activated
    stopSectionActive[local]=true;
    // save timestamp
    stopSectionActivated[local]=millis();
  }
}
/*
void setStopSection(int section) {
  // Set stop section to high
  Serial.println(F("Setting stop section with high=true"));
  setStopSection(section, true);
}
*/

void unsetStopSection(int section) {
  // use local variable, and subtract one from section to correspond with arrray
  byte local = section - 1;
  // unset stop section
  digitalWrite(stopSection[local],LOW);
  // set stop section as deactivated
  stopSectionActive[local]=false;
  // save timestamp
  stopSectionActivated[local]=0;  
}

bool isStopSectionActivated(int section) {
  // use local variable, and subtract one from section to correspond with arrray
  byte local = section - 1;
  // return the internal value
  return stopSectionActive[local];
}
/*
void iteratestopSection() {
  // iterate through stop sections
  // Note that while the arrays starts at 0, all functions starts at 1
  // for each stop section
  for (int currentStopSection=0; currentStopSection<_numStopSections-1; currentStopSection++) {
    // check if the turnout is activated
    if (isStopSectionActivated(currentStopSection+1)) {
      // if turnout is activated, check if current time is passed its duration time
      if (stopSectionActivated[currentStopSection] >= millis() + stopSectionDuration[currentStopSection]) {
        // if so, unset the turnout
        unsetStopSection(currentStopSection+1);
      }
    }
  }
}
*/

// End of signaling functions

/*
--------------------------------------
  7. ACCESSSORY FUNCTIONS
--------------------------------------

This section contain a list of accessory functons used by this URB.

Add your own functions here

*/

// End of signaling functions

/*
--------------------------------------
  8. YOUR FUNCTIONS
--------------------------------------

You may add any personal constants, variables, and functions, used
by this URB at this point

*/
void iterateFeedbacks() {
  // iterate though feedbacks
  // Note that while the arrays starts at 0, all functions starts at 1
  // for each feedback position
  for (int currentFeedback=0; currentFeedback<_numFeedbacks; currentFeedback++) {
    // Store pin value to internal array
    feedbacksActive[currentFeedback]=!digitalRead(feedbacks[currentFeedback]);
  }
}

// Defined SpeedArrays
byte const speedArray89[] = {50,59,68,77,86,95,104,113,121,130,139,148,157,166,175,184,193,202,211,220,229,238,247,255};
//byte const sizeOfSpeedArray89 = byte(sizeof(speedArray89)/sizeof(speedArray89[0]));

// Train states
enum _trainStates { 
  STATE_STOPPED, 
  STATE_ACCELERATING, 
  STATE_RUNNING, 
  STATE_DECELERATING, 
  STATE_STOPPING, 
  STATE_RELEASING
};

// Train variables
int const _numTrains = 1;
_trainStates trainState[_numTrains];
int trainHalting[_numTrains];
int trainInertia[_numTrains];
int trainSpeed[_numTrains];
int trainTargetSpeed[_numTrains];
unsigned long trainTimeStamp[_numTrains];

// When using enums, the function has to be predeclared
void initializeTrain(int train, enum _trainStates state , int haltingInS, int inertia);
void initializeTrain(int train, enum _trainStates state , int haltingInS, int inertia) {
  // use local variable, and subtract one from train to correspond with arrray
  byte local = train - 1;
  // store the variables in the arrays
  trainState[local]=state;
  trainHalting[local]=haltingInS;
  trainInertia[local]=inertia;
  // clear other arrays
  trainTargetSpeed[local]=0;
  trainTimeStamp[local]=0;
}
/*
void initializeTrain(int train, int haltingInS, int inertia) {
  initializeTrain(train, STATE_STOPPED, haltingInS, inertia);
}
void initializeTrain(int train, int haltingInS) {
  initializeTrain(train, STATE_STOPPED, haltingInS, 0);
}
*/

int getSpeed(int train) {
  int temp;
  // select trainspeed based on train #
  switch (train) {
    case 0:
      // Get train speed for train from its array
      temp=speedArray89[trainSpeed[train]];
      break;
    default:
      break;
  }
  // return the speed
  return temp;
}

void setTrainRunning(int train) {
  // set state to running
  trainState[train]=STATE_RUNNING;
  // Clear last movement
  trainTimeStamp[train]=0;
}

void changeTrainSpeed(int train, bool accellerate) {
  // if accellerating
  if (accellerate) {
    trainSpeed[train]++;
  } else {
    trainSpeed[train]--;
  }
  // execute that speed
  controlTrack(1,getSpeed(train));
  // store timestamp
  trainTimeStamp[train]=millis();          
}

bool feedbackIsActivated(int feedback) {
  // use local variable, and subtract one from train to correspond with arrray
  byte local = feedback - 1;
  return feedbacksActive[local];
}

// End of your functions

/*
--------------------------------------
  9. ARDUNIO SETUP FUNCTION
--------------------------------------

This section contains the Arduino setup function for this URB

Edit this section as required for your URB

*/

// debug variables
// if you turn debugging on, be sure to also uncomment Serial monitor
// in the setup function below
bool debug=false;
bool debugStateChanged[_numTrains];
bool debugSpeedChanged[_numTrains];

void setup() {
  /* 
  Anything placed in this function will be executed ONCE as soon as this 
  URB gets power.

  After this function has been executed to its end, the loop function
  will be executed repeatedly as long as this URB is powered.
  */

  // Serial monitor
  // Serial monitor must be available for debugging
  //Serial.begin(9600);
  //Serial.println();

  // Start debugging
  if (debug) {
    Serial.println(F("Debugging started"));
    // set all debugging arrays to their defaults
    for (int temp=0; temp<_numTrains; temp++) {
      debugStateChanged[temp]=true;
      debugSpeedChanged[temp]=true;
    }
  }
  
  // initialize features
  // Track #, EnablePin, PinA, PinB
  initializeTrack(1, 10, 5, 6);
  
  // Turnout #, Normal pin, Divergin pin, Duration
  initializeTurnout(1, 3, 4, 60);

  // Stop section #, Pin, Duration
  // Doesn't work if setStopSection is in use
  initializeStopSection(1, 2, 0);
  
  // Feedback #, Pin, Pullup (default is true)
  initializeFeedback(1,7, true); // inner
  initializeFeedback(2,8, true); // outer

  // setup defaults
  // Set all stopsections to stop
  // Doesn't work if initializeStopSection is in use
  setStopSection(1,true);

  // adding trains
  // Train #, state=STATE_STOPPED, halting in seconds, inertia in ms
  initializeTrain(1, STATE_STOPPED, 10, 300);

  // End of setup
  if (debug) {
    Serial.println(F("Init done"));
  }
}

// End of Arduino setup function

/*
--------------------------------------
 10. ARDUINO LOOP FUNCTION
--------------------------------------

This section contains the Arduino loop function for this URB

Edit this section as required for your URB

*/

// loop variables
// set currentTrain to the highest number of trains
// first time loop function is run, currentTrain is automatically
// set to 0 
int currentTrain=_numTrains;
// initate temp variable for use in loop function
unsigned long temp;

void loop() {
  /* 
  Everything placed in this function will be executed repeatedly
  as long as this URB is powered.
  
  The setup function will be executed before this function

  */
  
  // iterate turnouts to turn off motors
  iterateTurnouts();

  // iterate feedbacks to catch activations
  iterateFeedbacks();
/*
  // iterate trains to command trains
  // increase currentTrain
  currentTrain++;
  // if current train is greater or equal to number of trains
  if (currentTrain>=_numTrains) {
    // reset to current train to 0
    currentTrain=0;
  }
*/
  currentTrain=0;

  // Debug messages
  if (debug==true) {
    // if the state of the train was changed
    if (debugStateChanged[currentTrain]==true) {
      // send correct debug message to the serial monitor
      switch (trainState[currentTrain]) {
        case STATE_STOPPED:
          Serial.print("Train ");
          Serial.print(currentTrain+1);
          Serial.println(" is stopped");
          break;
        case STATE_ACCELERATING:
          Serial.print("Train ");
          Serial.print(currentTrain+1);
          Serial.println(" is accelerating");
          break;
        case STATE_RUNNING:
          Serial.print("Train ");
          Serial.print(currentTrain+1);
          Serial.println(" is running");
          break;
        case STATE_DECELERATING:
          Serial.print("Train ");
          Serial.print(currentTrain+1);
          Serial.println(" is decelerating");
          break;
        case STATE_STOPPING:
          Serial.print("Train ");
          Serial.print(currentTrain+1);
          Serial.println(" is stopping");
          break;
        case STATE_RELEASING:
          Serial.print("Train ");
          Serial.print(currentTrain+1);
          Serial.println(" is being released");
          break;
        default:
          Serial.println("Guess what?");
          Serial.println("Someone fucked up!");
          Serial.println();
          Serial.println("And it wasn't me");
          Serial.println();
          Serial.print("Train ");
          Serial.print(currentTrain+1);
          Serial.println(" not defined!");
          break;
      }
      // After sending debug message, turn off future messages
      debugStateChanged[currentTrain]=false;
    }
    // if speed was changed
    if (debugSpeedChanged[currentTrain]==true) {
      // send current speed to serial monitor
      Serial.print("Train ");
      Serial.print(currentTrain+1);
      Serial.print("'s current speed level is ");
      Serial.println(trainSpeed[currentTrain]);
      // after sending, turn off future messages
      debugSpeedChanged[currentTrain]=false;
    }
  }
  
  // checking state of current train
  switch (trainState[currentTrain]) {
    case STATE_STOPPED:
      // if train has never been moved
      if (trainTimeStamp[currentTrain]==0) {
        // set train state to releasing        
        trainState[currentTrain] = STATE_RELEASING;
        // if debugging
        if (debug==true) {
          // store the change to display debug message
          debugStateChanged[currentTrain]=true;
        }
      } else {
        // calculate holding time
        temp=(trainHalting[currentTrain]*1000)+trainTimeStamp[currentTrain];
        // if current time is greater or equal to holding time
        if (millis()>=temp) {
          // set train state to releasing        
          trainState[currentTrain]=STATE_RELEASING;
          // if debugging
          if (debug==true) {
            // store the change to display debug message
            debugStateChanged[currentTrain]=true;
          }
        }
      }
      break;
    case STATE_ACCELERATING:
      // if starting
      if (trainTimeStamp[currentTrain]==0) {
        // set current speed to level 0
        trainSpeed[currentTrain]=0;
        // accellerate train
        changeTrainSpeed(currentTrain, true);
        // if debugging
        if (debug==true) {
          // store the change to display debug message
          debugSpeedChanged[currentTrain]=true;
        }
      } else {
        // if target speed has been reached
        if (trainSpeed[currentTrain]==trainTargetSpeed[currentTrain]) {
          // set train state to running
          setTrainRunning(currentTrain);
          // if debugging
          if (debug==true) {
            // store the change to display debug message
            debugStateChanged[currentTrain]=true;
          }
        } else {
          // Add last movement to inertia, and store temporarily
          temp=trainTimeStamp[currentTrain]+trainInertia[currentTrain];
          // if that value is less than or equal to current time
          if (temp<=millis()) {
            // accellerate train
            changeTrainSpeed(currentTrain, true);
            // if debugging
            if (debug==true) {
              // store the change to display debug message
              debugSpeedChanged[currentTrain]=true;
            }
          }
        }
      }
      break;
    case STATE_RUNNING:
      // if feedback 1 (inner) is activated
      if (feedbackIsActivated(1)) {
        // set turnout 1 to normal
        setTurnout(1, true);
        // set train state to stopping
        trainState[currentTrain]=STATE_STOPPING;
        // if debugging
        if (debug==true) {
          // store the change to display debug message
          debugStateChanged[currentTrain]=true;
        }
      }
      // if feedback 2 (outer) is activated
      if (feedbackIsActivated(2)) {
        // set turnout 1 to diverging
        setTurnout(1, false);
        // set train state to decellerating 
        trainState[currentTrain]=STATE_DECELERATING;
        // if debugging
        if (debug==true) {
          // store the change to display debug message
          debugStateChanged[currentTrain]=true;
        }
        // set target speed to 0
        trainTargetSpeed[currentTrain]=0;
        // if debugging
        if (debug==true) {
          // store the change to display debug message
          debugSpeedChanged[currentTrain]=true;
        }
      }
      break;
    case STATE_DECELERATING:
      // if target speed has been reached
      if (trainSpeed[currentTrain]==trainTargetSpeed[currentTrain]) {
        // set train state to running
        setTrainRunning(currentTrain);
        // if debugging
        if (debug==true) {
          // store the change to display debug message
          debugStateChanged[currentTrain]=true;
        }
      } else {
        // Add last movement to inertia, and store temporarily
        temp=trainTimeStamp[currentTrain]+trainInertia[currentTrain];
        // if that value is less than or equal to current time
        if (temp<=millis()) {
          // deccellerate train
          changeTrainSpeed(currentTrain, false);
          // if debugging
          if (debug==true) {
            // store the change to display debug message
            debugSpeedChanged[currentTrain]=true;
          }
        }
      }
      break;
    case STATE_STOPPING:
      // if feedback 1 (inner) is activated
      if (feedbackIsActivated(1)) {
        // set stop section to high
        setStopSection(1,true);
        // set train state to stopped
        trainState[currentTrain]=STATE_STOPPED;
        // if debugging
        if (debug==true) {
          // store the change to display debug message
          debugStateChanged[currentTrain]=true;
        }
        // store timestamp        
        trainTimeStamp[currentTrain]=millis();
      }
      break;
    case STATE_RELEASING:
      // release the stop section
      unsetStopSection(1);
      // set the state to accellerating
      trainState[currentTrain]=STATE_ACCELERATING;
      // if debugging
      if (debug==true) {
        // store the change to display debug message
        debugStateChanged[currentTrain]=true;
      }
      // set the target speed to speed level 23
      trainTargetSpeed[currentTrain]=23;
      // if debugging
      if (debug==true) {
        // store the change to display debug message
        debugSpeedChanged[currentTrain]=true;
      }
      // set last movement to 0
      trainTimeStamp[currentTrain]=0;
      break;
    default:
      Serial.println("Looping error!");
      break;
  }
}

// End of Arduino loop section
// Do not add anything after this point
// End of sketch
