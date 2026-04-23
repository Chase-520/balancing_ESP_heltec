// #include <Arduino.h>
// // Turns the 'PRG' button into the power button, long press is off 
// #define HELTEC_POWER_BUTTON   // must be before "#include <heltec_unofficial.h>"

// // creates 'radio', 'display' and 'button' instances 
// #include <heltec_unofficial.h>
// #include <WiFi.h>
// #include <esp_now.h>
// #include "esp_wifi.h"
// typedef struct {
//   int value;
// } Data;

// Data data;

// void onReceive(const uint8_t * mac, const uint8_t *incomingData, int len) {
//   memcpy(&data, incomingData, sizeof(data));

//   Serial.print("Received: ");
//   Serial.println(data.value);
//   display.println(data.value);
//   display.display();
// }
// void setup() {
//   Serial.begin(115200);
//   heltec_setup();
//   display.displayOn();
//   display.println("Setting up receiver");
//   display.display();
//   WiFi.mode(WIFI_STA);
//   esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);
//   WiFi.disconnect();

//   if (esp_now_init() != ESP_OK) {
//     Serial.println("ESP-NOW init failed");
//     return;
//   }

//   esp_now_register_recv_cb(onReceive);
// }

// void loop() {}