#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

//
// Topology
//
RF24 radio(7,8);                                                    // Set up nRF24L01 radio on SPI bus plus pins 7 & 8

const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };      // Radio pipe addresses for the 2 nodes to communicate.

const int min_payload_size = 4;
const int max_payload_size = 32;
const int payload_size_increments_by = 1;
int next_payload_size = min_payload_size;

char receive_payload[max_payload_size+1]; // +1 to allow room for a terminating NULL char

void sendString(char* payload, int payload_size){
    // First, stop listening so we can talk.
    radio.stopListening();

    // Take the time, and send it.  This will block until complete
    Serial.print(F("Now sending length "));
    Serial.println(payload_size);
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

void setup(void)
{
  Serial.begin(115200);
  //printf_begin(); //Printf is used for debug
  
  Serial.println(F("RF24/examples/pingpair_dyn/"));

  
  radio.begin();                            // Setup and configure rf radio
  radio.enableDynamicPayloads();            // enable dynamic payloads
  radio.setRetries(5,15);                  // optionally, increase the delay between retries & # of retries

  // Open pipes to other nodes for communication
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);

  radio.startListening();
  radio.printDetails();
}


void loop(void)
{

    // The payload will always be the same, what will change is how much of it we send.
    static char send_payload[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ789012";

    sendString(send_payload, next_payload_size);

    // Update size for next time.
    next_payload_size += payload_size_increments_by;
    if ( next_payload_size > max_payload_size )
    next_payload_size = min_payload_size;

    // Try again 1s later
    delay(100);
}
// vim:cin:ai:sts=2 sw=2 ft=cpp
