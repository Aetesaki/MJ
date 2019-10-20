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
//
// Distanse til innkjør til skyggestasjon
const unsigned int DISTANSE_TIL_SKYGGESTASJON = 1500; // millisekunder

// variables
// tidsvariabler
unsigned long foresporselMottatt;
unsigned long signalRevet;
// andre variabler
char signalSettesTil;
bool sperret;

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
		// lagre tidspunkt passering for senere sammenlikning
		// (programvareforrigling)
		signalRevet=millis();
		// sett sperret sann for hard forrigling av togvei
		sperret=true;
		// sett signalbilde til stopp for sikring av togvei
		settSignalTil(SIGNALBILDE_STOP);
	}
	
	// Sjekk om signal er revet
	if ( signalRevet ) {
		// om signal er revet, sjekk tid mot distanse i tid frem til
		// innkjør til skyggestasjon
		if ( sjekkTidMot(signalRevet,SIGNALBILDE_TID_MS) ) {
			// om tog er ankommet skyggestasjon
			// kanseller programvareforrigling
			signalRevet=0;
		}
	}
	
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
	
	
}

bool sjekkTidMot(unsigned long lagretTid, unsigned long venteTid) {
	if ( millis() < lagretTid + venteTid ) 
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
	
	
	
