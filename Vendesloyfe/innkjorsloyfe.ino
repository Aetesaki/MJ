// Sketch for innkjør til vendesløyfe
// (c) Aetesaki 2019
// version 0.1 (UTESTET)

// pins must be changed depending on your device
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

// variables
bool signalRevet = false;
char signalSettesTil = SIGNALBILDE_AV;

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
	// Når tog passerer signal og signal for riving av signal 
	// går aktivt lavt
	if ( !digitalRead(RIVING) ) {
		// lagre passering for lokal forrigling av signal
		signalRevet=true;
		// sett signalbilde til stopp for sikring av togvei
		settSignalTil(SIGNALBILDE_STOP);
	}
	
	// Når signal for sperring går aktivt lavt
	if ( !digitalRead(SPERRING) )
		// slå av lokal forrigling av signal
		signalRevet=false;
	
	// Om forespørsel om innkjør til vendesløyfe mottas
	// (en eller begge inngangene går aktivt lavt)
	if ( !digitalRead(INNKJ) || !digitalRead(INNKJAV) ) {
		// lagre forespurt signalbilde
		if ( !digitalRead(INNKJ) ) 
			signalSettesTil=SIGNALBILDE_KJOR;
		if ( !digitalRead(INNKJAV) )
			signalSettesTil=SIGNALBILDE_LANGSOMT;
		// signalbilde settes kun til kjør dersom en utvetydig 
		// forespørsel mottas
	}
	
	// Sett signalbilde om alle forutsetninger ligger til rette
	//
	// hvis forespørsel er mottatt og hverken lokal forrigling eller
	// ekstern forrigling er aktivert
	if ( signalSettesTil && !signalRevet && digitalRead(SPERRING) ) {
		// sett signalbilde som forespurt,
		settSignalTil(signalSettesTil);
		// slett forespørsel
		signalSettesTil=SIGNALBILDE_AV;
		// og legg om veksel
		settVekselTil(VEKSEL_LAGT_OM);
	}
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
