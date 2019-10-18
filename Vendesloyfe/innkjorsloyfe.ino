// Sketch for innkjør til vendesløyfe
// (c) Aetesaki 2019
// version 0.1 (UTESTET)

// pins
// inputs
const int INNKJ = 1; // Forespørsel om innkjør til vendesløyfe
const int INNKJAV = 2; // – avvikende
const int SPERRING = 3; // Sperring av innkjør til vendesløyfe
const int RIVING = 4; // Signal for riving av signal
// outputs
const int VEKSEL = 5; // Utgang for å legge om veksel
const int SIGNAL_STOPP = 6; // Utgang for å sette signal til stopp
const int SIGNAL_KJOR = 7; // Utgang for å sette signal til kjør
const int SIGNAL_LANGSOMT = 8; // Utgang for å sette signal til langsomt

// constants
// Signalkonstanter
const char SIGNALBILDE_AV = 0;
const char SIGNALBILDE_STOP = 1;
const char SIGNALBILDE_KJOR = 2;
const char SIGNALBILDE_LANGSOMT = 3;
// Tid for puls for å legge om signal
const unsigned int SIGNALBILDE_TID_MS = 0; // millisekunder
//
// Vekselkonstanter
const char VEKSEL_USATT = 0;
const char VEKSEL_LAGT_OM = 1;
// Tid for puls for å legge om veksel
const unsigned int VEKSEL_TID_MS = 500; // millisekunder
//
// Distanse til innkjør til skyggestasjon
const unsigned int DISTANSE_TIL_SKYGGESTASJON = 1500; // millisekunder

// variables
// tidsvariabler
unsigned long foresporselMottatt;
unsigned long signalRevet;
// andre variabler
char signalSettesTil;
char flags; 

void setup() {
	// setup input pins
	pinMode(INNKJ, INPUT_PULLUP);
	pinMode(INNKJAV, INPUT_PULLUP);
	pinMode(SPERRING, INPUT_PULLUP);
	pinMode(RIVING, INPUT_PULLUP);
	// setup output pins
	pinMode(VEKSEL, OUTPUT);
	pinMode(SIGNAL_KJOR, OUTPUT);
	pinMode(SIGNAL_LANGSOMT, OUTPUT);
	pinMode(SIGNAL_STOPP, OUTPUT);
	// set outpins to default
	settVekselTil(VEKSEL_USATT); // sett veksel til usatt
	settSignalTil(SIGNALBILDE_STOP); // sett signal til stopp
}

void loop() {
	
}

bool sjekkTidMot(unsigned long tid) {
	if (millis() < tid) 
		return false;
	else
		return true;
}

void settVekselTil(char retning) {
	switch (retning) {
	case VEKSEL_LAGT_OM:
		digitalWrite(VEKSEL, HIGH);
		delay(VEKSEL_TID_MS);
	default:
		digitalWrite(VEKSEL, LOW);
		break;
	}
}		

void settSignalTil(char bilde) {
	switch (bilde) {
	case SIGNALBILDE_STOP:
		digitalWrite(SIGNAL_STOPP, HIGH);
		digitalWrite(SIGNAL_KJOR, LOW);
		digitalWrite(SIGNAL_LANGSOMT, LOW);
		if (SIGNALBILDE_TID_MS) {
			delay(SIGNALBILDE_TID_MS);
			settSignalTil(SIGNALBILDE_AV); 
		}
		break;
	case SIGNALBILDE_KJOR:
		digitalWrite(SIGNAL_STOPP, LOW);
		digitalWrite(SIGNAL_KJOR, HIGH);
		digitalWrite(SIGNAL_LANGSOMT, LOW);
		if (SIGNALBILDE_TID_MS) {
			delay(SIGNALBILDE_TID_MS);
			settSignalTil(SIGNALBILDE_AV); 
		}
		break;
	case SIGNALBILDE_LANGSOMT:
		digitalWrite(SIGNAL_STOPP, LOW);
		digitalWrite(SIGNAL_KJOR, HIGH);
		digitalWrite(SIGNAL_LANGSOMT, HIGH);
		if (SIGNALBILDE_TID_MS) {
			delay(SIGNALBILDE_TID_MS);
			settSignalTil(SIGNALBILDE_AV); 
		}
		break;
	default:
		digitalWrite(SIGNAL_STOPP, LOW);
		digitalWrite(SIGNAL_KJOR, LOW);
		digitalWrite(SIGNAL_LANGSOMT, LOW);
		break;
	}
}
	
	
	
