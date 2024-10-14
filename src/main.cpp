#include <Arduino.h>

// put function declarations here:
// KAMY
#define LDR A0 // LDR at port A0
int ldr;
int LED = 13; // On-board LED at 13
int RELAY = 2; // RELAY at port 2

void setup() 
{
  // put your setup code here, to run once:
 pinMode(LED, OUTPUT); // LED is output
 pinMode(RELAY, OUTPUT); // RELAY is output
 digitalWrite(LED, LOW); // LED OFF at beginning
 digitalWrite(RELAY, LOW); // RELAY OFF at beginning
}

void loop() {
  // put your main code here, to run repeatedly:
}

void LIGHT()
{
  ldr = analogRead(LDR); // Read temperature
  if(ldr > 800) // If dark detected
  {
  digitalWrite(LED, HIGH); // LED ON
  digitalWrite(RELAY, HIGH); // RELAY ON
  }
  else
  {
  digitalWrite(LED, LOW); // LED OFF
  digitalWrite(RELAY, LOW); // RELAY OFF
  }
}