// Sketch for innkjør til vendesløyfe
// (c) Aetesaki 2019
// version 0.2 (KOMPILERT, MEN UTESTET)

// pins must be changed depending on your device
// inputs 
const int FORRIGLING_INN = 2; // inngang for forrigling fra togspor
const int TOGSPOR_BESATT = 3; // inngang for fulle togspor
// outputs
const int FORRIGLING_UT	= 4; // utgang for forrigling av innkjør
const int SIGNAL_SIMULERT = 13; // utgang for simulert signal

// constants
const bool SIGNALBILDE_STOP = false;
const bool SIGNALBILDE_KJOR = true;

// variabler
bool sisteTogAnkommet = false;

// funksjoner
void settSimulertSignalTil(bool bilde) {
	digitalWrite(SIGNAL_SIMULERT, bilde);
}

// setup
void setup() {
	pinMode(FORRIGLING_INN, INPUT_PULLUP);
	pinMode(TOGSPOR_BESATT, INPUT_PULLUP);
	pinMode(FORRIGLING_UT, OUTPUT);
	pinMode(SIGNAL_SIMULERT, OUTPUT);
	settSimulertSignalTil(SIGNALBILDE_STOP);
}

// loop
void loop() {
	// hvis togspor er besatt
	if (!digitalRead(TOGSPOR_BESATT)) {
		// sett signal til stop
		settSimulertSignalTil(SIGNALBILDE_STOP);
		// send forrigling til innkjør til vendesløyfe
		digitalWrite(FORRIGLING_UT, LOW);
	}
	// hvis togspor ikke er besatt
	else {
		// og siste tog er ankommet
		if (sisteTogAnkommet) {
			// sett signal til kjør
			settSimulertSignalTil(SIGNALBILDE_KJOR);
			// opphev forrigling av innkjør tin vendesløyfe
			digitalWrite(FORRIGLING_UT, HIGH);
		}
	}
	// hvis siste tog ikke er ankommet
	if (!sisteTogAnkommet) 
		// og forrigling inn er mottatt
		if (!digitalRead(FORRIGLING_INN))
			// sett siste tog ankommet
			sisteTogAnkommet=true;
}
