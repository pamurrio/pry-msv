#include <Arduino.h>
#include "MAX30100_PulseOximeter.h" //libreria sensor oximetro
#include <Wire.h>
#include "oximeter.h" 
//// oximetro
PulseOximeter pox;// intanciamos el oximetro
uint32_t tsLastReport = 0;

void setupOximeter(){
	pox.begin(); // prende el oximetro y devuelve un booleano

	// oximetro
	// Initialize the PulseOximeter instance
	// Failures are generally due to an improper I2C wiring, missing power supply
	// or wrong target chip
	if (!pox.begin()) {
		Serial.println("FAILED");
	} else {
		Serial.println("SUCCESS");
	}

	pox.setOnBeatDetectedCallback(onBeatDetected);
}

//// Callback (registered below) fired when a pulse is detected
////
void onBeatDetected(){
 Serial.println("Beat!");
}

void executeOximeter(){
    //ejecuto oximetro
    // Make sure to call update as fast as possible
	pox.update();
    
	// Asynchronously dump heart rate and oxidation levels to the serial
	// For both, a value of 0 means "invalid"
	if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
		Serial.print("Heart rate:");
		Serial.print(pox.getHeartRate());
		Serial.print("bpm / SpO2:");
		Serial.print(pox.getSpO2());
		Serial.println("%");

		tsLastReport = millis();
	}
}