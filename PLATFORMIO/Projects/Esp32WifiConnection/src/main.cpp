#include <Arduino.h>
#include <WiFiMulti.h>
#include <WiFi.h>


#define WIFI_SSID "TP-Link_A6B0" // Ensure you connect to the 2.4 GHz network , 5G won't work  
#define WIFI_PSWD "Wi%416%Fi"

WiFiMulti wifiMulti;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

  Serial.println("WiFi in STATION mode");
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);

  while(1){
    Serial.print(".");
    int n = WiFi.scanNetworks();
    if(n!=0){
        Serial.print(n); Serial.println(" network(s) found");
        for(int i=0; i<n; i++){
            Serial.print("network "); Serial.print(i + 1); Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" ("); Serial.print(WiFi.RSSI(i)); Serial.print(")");
            Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)? " open ":" ***");
        }
        break;
    }
    else {
      Serial.println(" No network found");
    }
  }


  Serial.println("Connect to " WIFI_SSID);
  wifiMulti.addAP( WIFI_SSID, WIFI_PSWD );
  Serial.println("Connecting...");

  while(wifiMulti.run() != WL_CONNECTED) {
    Serial.println("[WIFI] Not connected!");
    delay(500);
    Serial.print(".");
  }
  Serial.print("WiFi connected. IP: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  digitalWrite(LED_BUILTIN, WiFi.status() == WL_CONNECTED);
}

