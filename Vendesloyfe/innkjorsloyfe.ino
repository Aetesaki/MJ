// Sketch for innkjør til vendesløyfe
// (c) Aetesaki 2019
// version 0.2 (UTESTET)

// pins must be changed depending on your device
// inputs 
const int FORESPORSEL_KJOR = 1; // Forespørsel om innkjør til vendesløyfe
const int FORESPORSEL_AVVIKENDE = 2; // – avvikende
const int FORRIGLING_INN = 3; // Innkommende forrigling
const int FORRIGLING_LOKAL = 4; // Lokal forrigling
// outputs
const int VEKSEL = 5; // Utgang for å legge om veksel
const int SIGNAL_STOPP = 6; // Utgang for å sette signal til stopp
const int SIGNAL_KJOR = 7; // – kjør
const int SIGNAL_AVVIKENDE = 8; // – avvikende

// constants
// Signalkonstanter
const char SIGNALBILDE_AV = 0;
const char SIGNALBILDE_STOP = 1;
const char SIGNALBILDE_KJOR = 2;
const char SIGNALBILDE_AVVIKENDE = 3;
// Tid for puls for å legge om signal
const unsigned int SIGNALBILDE_PULS_MS = 0; // millisekunder
//
// Vekselkonstanter
const char VEKSEL_NORMAL = 0;
const char VEKSEL_LAGT_OM = 1;
// Tid for puls for å legge om veksel
const unsigned int VEKSEL_PULS_MS = 500; // millisekunder

// variables
bool lokalForrigling = false;
char signalSettesTil = SIGNALBILDE_AV;

void setup() {
	// setup input pins
	pinMode(FORESPORSEL_KJOR, INPUT_PULLUP);
	pinMode(FORESPORSEL_AVVIKENDE, INPUT_PULLUP);
	pinMode(FORRIGLING_INN, INPUT_PULLUP);
	pinMode(FORRIGLING_LOKAL, INPUT_PULLUP);
	// setup output pins
	pinMode(VEKSEL, OUTPUT);
	pinMode(SIGNAL_KJOR, OUTPUT);
	pinMode(SIGNAL_AVVIKENDE, OUTPUT);
	pinMode(SIGNAL_STOPP, OUTPUT);
	// set outpins to default
	settVekselTil(VEKSEL_NORMAL); // sett veksel til usatt
	settSignalTil(SIGNALBILDE_STOP); // sett signal til stopp
}

void loop() {
	// Når inngang for lokal forrigling går aktivt lavt
	if ( !digitalRead(FORRIGLING_LOKAL) ) {
		// sett lokal forrigling
		lokalForrigling=true;
		// og sett signalbilde til stopp for sikring av togvei
		settSignalTil(SIGNALBILDE_STOP);
	}
	
	// Når signal for inngående forrigling går aktivt lavt
	if ( !digitalRead(FORRIGLING_INN) )
		// slå av lokal forrigling
		lokalForrigling=false;
	
	// Om forespørsel om innkjør til vendesløyfe mottas
	// (en eller begge inngangene går aktivt lavt)
	if ( !digitalRead(FORESPORSEL_KJOR) || !digitalRead(FORESPORSEL_AVVIKENDE) ) {
		// lagre forespurt signalbilde
		if ( !digitalRead(FORESPORSEL_KJOR) ) 
			signalSettesTil=SIGNALBILDE_KJOR;
		if ( !digitalRead(FORESPORSEL_AVVIKENDE) )
			signalSettesTil=SIGNALBILDE_AVVIKENDE;
		// signalbilde settes kun til kjør dersom en utvetydig 
		// forespørsel mottas
	}
	
	// Sett signalbilde om alle forutsetninger ligger til rette
	//
	// hvis forespørsel er mottatt og hverken lokal forrigling eller
	// ekstern forrigling er aktivert
	if ( signalSettesTil && !lokalForrigling && digitalRead(FORRIGLING_INN) ) {
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
		delay(VEKSEL_PULS_MS);
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
		digitalWrite(SIGNAL_AVVIKENDE, LOW);
		if (SIGNALBILDE_PULS_MS) {
			delay(SIGNALBILDE_PULS_MS);
			settSignalTil(SIGNALBILDE_AV); 
		}
		break;
	case SIGNALBILDE_KJOR:
		digitalWrite(SIGNAL_STOPP, LOW);
		digitalWrite(SIGNAL_KJOR, HIGH);
		digitalWrite(SIGNAL_AVVIKENDE, LOW);
		if (SIGNALBILDE_PULS_MS) {
			delay(SIGNALBILDE_PULS_MS);
			settSignalTil(SIGNALBILDE_AV); 
		}
		break;
	case SIGNALBILDE_LANGSOMT:
		digitalWrite(SIGNAL_STOPP, LOW);
		digitalWrite(SIGNAL_KJOR, HIGH);
		digitalWrite(SIGNAL_AVVIKENDE, HIGH);
		if (SIGNALBILDE_PULS_MS) {
			delay(SIGNALBILDE_PULS_MS);
			settSignalTil(SIGNALBILDE_AV); 
		}
		break;
	default:
		digitalWrite(SIGNAL_STOPP, LOW);
		digitalWrite(SIGNAL_KJOR, LOW);
		digitalWrite(SIGNAL_AVVIKENDE, LOW);
		break;
	}
}
