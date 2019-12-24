
const byte defaultSpeedArray[] = {20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,255};
const byte DIRECTION_FORWARD = 1;
const byte DIRECTION_BACKWARDS = 2;

// arrays of L298 pins
int _L298EnablePin[10];
int _L298PinA[10];
int _L298PinB[10];

// Points
int const point1inside = 4;
int const point1outside = 3;

// Stoppsection
int const stopSection1 = 2;

// Track feedback section
int const feedbackInner = 7;
int const feedbackOuter = 8;

// SpeedArrays
byte const speedArray[] = {50,59,68,77,86,95,104,113,121,130,139,148,157,166,175,184,193,202,211,220,229,238,247,255};
int const sizeOfSpeedArray = int(sizeof(speedArray));

// other constants
int const pointsDelay = 75;
int const latchDelay = 250;

// variables
bool test=false;

void initializeTrack(byte track, int enablePin, int  pinA, int pinB) {
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
  // Stop track, and set it to direction forward
  stopTrack(track);
  reverseTrack(track,DIRECTION_FORWARD);
}

// functions for controlling the tracks
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
 

// functions for controlling points
void setPoint(int pin, int delayInms) {
  digitalWrite(pin, HIGH);
  delay(delayInms);
  digitalWrite(pin, LOW);  
}

void setup() {

  // serial monitor
  Serial.begin(9600);

  // put your setup code here, to run once:
  Serial.print("Initializig track...\n");
  initializeTrack(1, 10, 5, 6);

  // Set PWM frequency for D9 & D10
  // Timer 1 divisor to 256 for PWM frequency of 122.55 Hz
  TCCR1B = TCCR1B & B11111000 | B00000100;     

  // local pins
  pinMode(point1inside,OUTPUT);
  pinMode(point1outside,OUTPUT);
  
  pinMode(stopSection1,OUTPUT);

  pinMode(feedbackInner,INPUT_PULLUP);
  pinMode(feedbackOuter, INPUT_PULLUP);

  // setup defaults
  // Set all stopsections to stopp
  digitalWrite(stopSection1,HIGH);
  // Set all points to their defaults
  Serial.print("Setting points...\n");
  setPoint(point1outside, pointsDelay);
  setPoint(point1inside, pointsDelay);

  // Start test
  delay(2000);

  // Start train
  digitalWrite(stopSection1, LOW);
  Serial.print("Releasing stopsection\n");
  delay(1000);

  for (int i = 0; i < sizeOfSpeedArray; i++) {
    controlTrack(1,speedArray[i]);
    Serial.print(speedArray[i]);
    Serial.print("\n");
    delay(300);
  }
}

// test variables
bool outerHit = false;
bool readyToStop = false;
bool holdStopSection = false;
int stopping=sizeOfSpeedArray;
bool halt=false;

void loop() {
  // put your main code here, to run repeatedly:
  if (!halt) {
    if (!digitalRead(feedbackInner)) {
      if (holdStopSection) {
        digitalWrite(stopSection1,HIGH);
        Serial.print("Stop section held\n");
        halt=true;
      } else {
        Serial.print("Inner feedback track\n");
        if (outerHit) {
          readyToStop = true;
          Serial.write("Ready to stop\n");
        } else {
          setPoint(point1outside, pointsDelay);
        }
      }
      delay(latchDelay);
    }
    if (!digitalRead(feedbackOuter)) {
      setPoint(point1inside, pointsDelay);
      outerHit = true;
      Serial.print("Outer feedback track\n");
      delay(latchDelay);
    }
    if (readyToStop) {
      if (stopping<1) {
        readyToStop=false;
        holdStopSection=true;
      } else {
        stopping=stopping-1;
        controlTrack(1,speedArray[stopping]);
        Serial.print(speedArray[stopping]);
        Serial.print("\n");
        delay(300);
      }
    }
  }
}
