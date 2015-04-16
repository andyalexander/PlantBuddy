#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <avr/sleep.h> 		// sleep library
#include <DHT.h> 			// DHT sensor library
#include <LowPower.h> 		// low power library

// Soil moisture sensor define
#define moistPIN1 A2 		// soil probe pin 1 with 56kohm resistor
#define moistPIN2 9 		// soil probe pin 2 with 100ohm resistor
#define moistREADPIN1 A0 	// analog read pin. connected to A2 PIN with 56kohm resistor


// DHT Humidity + Temperature sensor define
#define DHTPIN 5 			// Data pin (D5) for DHT
#define DHTPWR 4 			// turn DHT on and off via transistor
#define DHTTYPE DHT11 		// sensor model DHT11
DHT dht(DHTPIN, DHTTYPE); 	// define DHT11


// Battery meter
#define VoltagePinRead A7 	// analogue voltage read pin for batery meter
#define VoltagePinEnable A3 // current sink pin. ( enable voltage divider )
#define VoltageRef 3.3 		// reference voltage on system. use to calculate voltage from ADC
#define VoltageDivider 2 	// if you have a voltage divider to read voltages, enter the multiplier here.
int VoltageLow = 4; 		// low battery threshhold. 4 volts.
int VoltageADC;


// LED Pin
#define led 6

//// RADIO SETTINGS
#define NODEID        20  	// The ID of this node. Has to be unique. 1 is reserved for the gateway!
//#define NETWORKID    20  	//the network ID we are on
//#define GATEWAYID     1  	//the gateway Moteino ID (default is 1)
//#define ACK_TIME     2800  // # of ms to wait for an ack


// Power Management Sleep cycles
int sleepCycledefault = 10; // Sleep cycle 450*8 seconds = 1 hour. DEFAULT 450
int soilMoistThresh = 250; 	// soil moisture threshold. reference chart

String senseDATA; // sensor data STRING
String ErrorLvl = "0"; 		// Error level. 0 = normal. 1 = soil moisture, 2 = Temperature , 3 = Humidity, 4 = Battery voltage


// Need an instance of the Radio Module
RF24 radio(7,8);                                                    // Set up nRF24L01 radio on SPI bus plus pins 7(ce) & 8(cs)
bool requestACK=true;
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };      	// Radio pipe addresses for the 2 nodes to communicate.

const int min_payload_size = 4;
const int max_payload_size = 32;
const int payload_size_increments_by = 1;
int next_payload_size = min_payload_size;

char receive_payload[max_payload_size+1]; 							// +1 to allow room for a terminating NULL char


// LED Pulse function
void LEDPulse() {
	int i;
	delay (88);
	for (int i = 0; i < 88; i++) { 									// loop from 0 to 254 (fade in)
		analogWrite(led, i);      									// set the LED brightness
		delay(12);
	}

	for (int i = 88; i > 0; i--) { 									// loop from 255 to 1 (fade out)
		analogWrite(led, i); 										// set the LED brightness
		delay(12);
	}
	digitalWrite(led, LOW);
	delay (128);
}

// Moisture sensor reading function
// function reads 3 times and averages the data
int moistREAD() {
	int moistREADdelay = 88; // delay to reduce capacitive effects
	int moistAVG = 0;

	// polarity 1 read
	digitalWrite(moistPIN1, HIGH);
	digitalWrite(moistPIN2, LOW);
	delay (moistREADdelay);
	int moistVal1 = analogRead(moistREADPIN1);

	//	Serial.println(moistVal1);
	digitalWrite(moistPIN1, LOW);
	delay (moistREADdelay);

	// polarity 2 read
	digitalWrite(moistPIN1, LOW);
	digitalWrite(moistPIN2, HIGH);
	delay (moistREADdelay);
	int moistVal2 = analogRead(moistREADPIN1);

	//Make sure all the pins are off to save power
	digitalWrite(moistPIN2, LOW);
	digitalWrite(moistPIN1, LOW);
	moistVal1 = 1023 - moistVal1; // invert the reading
	//	Serial.println(moistVal2);
	moistAVG = (moistVal1 + moistVal2) / 2; // average readings. report the levels
	return moistAVG;
}

// LED Blink function
void LEDBlink(int DELAY_MS)
{
	digitalWrite(led,HIGH);
	delay(DELAY_MS);
	digitalWrite(led,LOW);
	delay(DELAY_MS);
}

void sendString(char* payload, int payload_size){
	// First, stop listening so we can talk.
	radio.stopListening();

	// Take the time, and send it.  This will block until complete
	Serial.print(F("Now sending length "));
	Serial.println(payload_size);
	Serial.println(payload);
	radio.write(payload, payload_size);

	// Now, continue listening
	radio.startListening();

	// Wait here until we get a response, or timeout
	unsigned long started_waiting_at = millis();
	bool timeout = false;
	while ( ! radio.available() && ! timeout )
		if (millis() - started_waiting_at > 500 )
			timeout = true;

	// Describe the results
	if ( timeout )
	{
		Serial.println(F("Failed, response timed out."));
	}
	else
	{
		// Grab the response, compare, and send to debugging spew
		uint8_t len = radio.getDynamicPayloadSize();

		// If a corrupt dynamic payload is received, it will be flushed
		if(!len){
			return;
		}

		radio.read( receive_payload, len );

		// Put a zero at the end for easy printing
		receive_payload[len] = 0;

		// Spew it
		Serial.print(F("Got response size="));
		Serial.print(len);
		Serial.print(F(" value="));
		Serial.println(receive_payload);
	}


}

float checkVoltage(void){
	// Battery level check
	pinMode(VoltagePinEnable, OUTPUT); // change pin mode
	digitalWrite(VoltagePinEnable, LOW); // turn on the battery meter (sink current)
	for ( int i = 0 ; i < 3 ; i++ ) {
		delay(48); // delay, wait for circuit to stabalize
		VoltageADC = analogRead(VoltagePinRead); // read the voltage 3 times. keep last reading
	}
	float Voltage = ((VoltageADC * VoltageRef) / 1023) * VoltageDivider; // calculate the voltage
	if (Voltage < VoltageLow){
		ErrorLvl = "4"; // assign error level
	}
	pinMode(VoltagePinEnable, INPUT); // turn off the battery meter
	Serial.print("Battery Level: ");
	Serial.println(Voltage);
	return Voltage;
}

void setup(void)
{
	Serial.begin(115200);

	radio.begin();                            // Setup and configure rf radio
	radio.enableDynamicPayloads();            // enable dynamic payloads
	radio.setRetries(5,15);                  	// optionally, increase the delay between retries & # of retries

	// Open pipes to other nodes for communication
	radio.openWritingPipe(pipes[0]);
	radio.openReadingPipe(1,pipes[1]);

	radio.startListening();					// start listening
	radio.printDetails();						// get the details of the radio
	//  radio.powerDown();						// move to low power state

	pinMode(led, OUTPUT);						//LED setup.

	// Battery Meter setup
	pinMode(VoltagePinRead, INPUT);
	pinMode(VoltagePinEnable, INPUT);

	// Moisture sensor pin setup
	pinMode(moistPIN1, OUTPUT);
	pinMode(moistPIN2, OUTPUT);
	pinMode(moistREADPIN1, INPUT);

	// Humidity sensor setup
	pinMode(DHTPWR, OUTPUT);
	dht.begin();

	// power on indicator
	LEDBlink(80);
	LEDBlink(80);
}

int readMoisture(void){
	// Soil Moisture sensor reading
	int moistREADavg = 0; // reset the moisture level before reading
	int moistCycle = 3; // how many times to read the moisture level. default is 3 times
	for ( int moistReadCount = 0; moistReadCount < moistCycle; moistReadCount++ ) {
		moistREADavg += moistREAD();
	}
	moistREADavg = moistREADavg / moistCycle; // average the results
	Serial.print("Soil Moisture: ");
	Serial.println(moistREADavg);


	// if soil is below threshold, error level 1
	if ( moistREADavg < soilMoistThresh ) {
		ErrorLvl += "1"; // assign error level
		LEDBlink(128);
		LEDBlink(128);
		LEDBlink(128);
	}
}

void loop(void){
	int sleepCYCLE = sleepCycledefault; // Sleep cycle reset
	ErrorLvl = "0"; // Reset error level


	float Voltage = checkVoltage();
	int moistREADavg = readMoisture();


	// Humidity + Temperature sensor reading
	// Reading temperature or humidity takes about 250 milliseconds!
	// Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
	digitalWrite(DHTPWR, HIGH); // turn on sensor
	delay (38); // wait for sensor to stabalize
	int dhttempc = dht.readTemperature(); // read temperature as celsius
	int dhthumid = dht.readHumidity(); // read humidity
	//Serial.println(dhttempc);

	// check if returns are valid, if they are NaN (not a number) then something went wrong!
	if (isnan(dhttempc) || isnan(dhthumid) || dhttempc == 0 || dhthumid == 0 ) {
		dhttempc = 0;
		dhthumid = 0;
		ErrorLvl += "23";
	}
	delay (18);
	digitalWrite(DHTPWR, LOW); // turn off sensor
	Serial.print("Temperature in C: ");
	Serial.println(dhttempc);
	Serial.print("Humidity in percent: ");
	Serial.println(dhthumid);


	// PREPARE READINGS FOR TRANSMISSION
	senseDATA = String(NODEID);
	senseDATA += ":";
	senseDATA += ErrorLvl;
	senseDATA += ":";
	senseDATA += String(moistREADavg);
	senseDATA += ":";
	senseDATA += String(dhttempc);
	senseDATA += ":";
	senseDATA += String(dhthumid);
	senseDATA += ":";
	char VoltagebufTemp[10];
	dtostrf(Voltage,5,3,VoltagebufTemp); // convert float Voltage to string
	senseDATA += VoltagebufTemp;
	byte sendSize = senseDATA.length();
	sendSize = sendSize + 1;
	char sendBuf[sendSize];
	senseDATA.toCharArray(sendBuf, sendSize); // convert string to char array for transmission


	//Transmit the data
	LEDPulse (); // pulse the LED
	radio.powerUp();
	sendString(sendBuf, sendSize);

	radio.powerDown();



	// Error Level handing
	// If any error level is generated, halve the sleep cycle
	//if ( ErrorLvl.toInt() > 0 ) {
	//  sleepCYCLE = sleepCYCLE / 2;
	//  LEDBlink(30);
	//  LEDBlink(30);
	//  LEDBlink(30);
	// }




	// Randomize sleep cycle a little to prevent collisions with other nodes
	//sleepCYCLE = sleepCYCLE + random(8);



	// POWER MANAGEMENT DEEP SLEEP
	// after everything is done, go into deep sleep to save power
	for ( int sleepTIME = 0; sleepTIME < sleepCYCLE; sleepTIME++ ) {
		LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF); //sleep duration is 8 seconds multiply by the sleep cycle variable.
	}
}







