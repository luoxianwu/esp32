#include <Arduino.h>
#include <WiFiMulti.h>

#define WIFI_SSID "TP-Link_A6B0" // Ensure you connect to the 2.4 GHz network , 5G won't work  
#define WIFI_PSWD "Wi%416%Fi"

WiFiMulti wifiMulti;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

  Serial.println("add access point");
  wifiMulti.addAP( WIFI_SSID, WIFI_PSWD );
  Serial.println("Connecting...");

  while(wifiMulti.run() != WL_CONNECTED) {
    Serial.println("[WIFI] Not connected!");
    delay(500);
  }
    Serial.println("WiFi connected");
}

void loop() {
  digitalWrite(LED_BUILTIN, WiFi.status() == WL_CONNECTED);
}

