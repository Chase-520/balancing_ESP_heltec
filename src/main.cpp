#include <Arduino.h>
// Turns the 'PRG' button into the power button, long press is off 
#define HELTEC_POWER_BUTTON   // must be before "#include <heltec_unofficial.h>"

// creates 'radio', 'display' and 'button' instances 
#include <heltec_unofficial.h>
#include <helpers.h>
void setup() {
  Serial.begin(115200);
  heltec_setup();
  display.displayOn();
  display.println("Something");
  checkMacAddress();
}

void loop() {}