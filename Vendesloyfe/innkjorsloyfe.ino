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
const char SIGNALBILDE_AV = 0;
const char SIGNALBILDE_STOP = 1;
const char SIGNALBILDE_KJOR = 2;
const char SIGNALBILDE_LANGSOMT = 3;

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
	digitalWrite(VEKSEL, FALSE); // sett veksel til usatt
	settSignalTil(SIGNALBILDE_STOP); // sett signal til stopp
}

void settSignalTil(char bilde) {
	switch (bilde) {
	case SIGNALBILDE_STOP:
		digitalWrite(SIGNAL_STOPP, HIGH);
		digitalWrite(SIGNAL_KJOR, LOW);
		digitalWrite(SIGNAL_LANGSOMT, LOW);
		break;
	case SIGNALBILDE_KJOR:
		digitalWrite(SIGNAL_STOPP, LOW);
		digitalWrite(SIGNAL_KJOR, HIGH);
		digitalWrite(SIGNAL_LANGSOMT, LOW);
		break;
	case SIGNALBILDE_LANGSOMT:
		digitalWrite(SIGNAL_STOPP, LOW);
		digitalWrite(SIGNAL_KJOR, HIGH);
		digitalWrite(SIGNAL_LANGSOMT, HIGH);
		break;
	default:
		digitalWrite(SIGNAL_STOPP, LOW);
		digitalWrite(SIGNAL_KJOR, LOW);
		digitalWrite(SIGNAL_LANGSOMT, LOW);
		break;
	}
}
	
	
	
