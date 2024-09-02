#include <Arduino.h>
#include <WiFiMulti.h>
#include <WiFi.h>


#define WIFI_SSID "TP-Link_A6B0" // Ensure you connect to the 2.4 GHz network , 5G won't work  
#define WIFI_PSWD "Wi%416%Fi"

const int LedPin = 2;
WiFiServer server(80);
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
  //start web server
  server.begin();
  Serial.println("You can start a web browser, and visit the IP");
}

void loop() {
  WiFiClient client = server.accept();  // listen for incoming clients
  //when there is no activities on browser side, client is 0
  if (client) {                     // if you get a client,
    Serial.println("New Client.");  // print a message out the serial port
    String currentLine = "";        // make a String to hold incoming data from the client
    while (client.connected()) {    // loop while the client's connected
      if (client.available()) {     // if there's bytes to read from the client,
        char c = client.read();     // read a byte, then
        Serial.write(c);            // print it out the serial monitor
        if (c == '\n') {            // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            Serial.println("respose Page.");
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/H\">here</a> to turn the LED on pin 2 on.<br>");
            client.print("Click <a href=\"/L\">here</a> to turn the LED on pin 2 off.<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {  // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(LedPin, HIGH);  // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(LedPin, LOW);  // GET /L turns the LED off
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}

