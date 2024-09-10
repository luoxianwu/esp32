#include <WiFi.h>
#include <WebServer.h>

String getHTML();
void handle_OnConnect();
void handle_ledON();
void handle_ledOFF();
void handle_NotFound();

#define ssid "ESP32_AP"
#define password "12345678"

#define blueLedPin 2

IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
WebServer server(80);

bool LEDStatus = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(blueLedPin, OUTPUT);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(1000);

  server.on("/", handle_OnConnect);
  server.on("/ledon", handle_ledON);
  server.on("/ledoff", handle_ledOFF);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP Server Started");
}

void handle_OnConnect() {
    LEDStatus = LOW;
    Serial.println("LED Status: OFF");
    server.send(200, "text/html", getHTML());
}

void handle_ledON() {
    LEDStatus = HIGH;
    Serial.println("LED Status: ON");
    server.send(200, "text/html", getHTML());
}

void handle_ledOFF() {
    LEDStatus = LOW;
    Serial.println("LED Status: OFF");
    server.send(200, "text/html", getHTML());
}

void handle_NotFound() {
    server.send(404, "text/plain", "Not Found");
}


void loop() {
    server.handleClient(); // Handles incoming client requests
    if (LEDStatus) {
        digitalWrite(blueLedPin, HIGH); // Turns the blue LED on if LEDStatus is true
    } else {
        digitalWrite(blueLedPin, LOW); // Turns the blue LED off if LEDStatus is false
    }
}



String getHTML() {
    String htmlcode = "<!DOCTYPE html> <html>\n";
    htmlcode += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
    htmlcode += "<title>LED Control</title>\n";
    
    htmlcode += "<style>html { font-family: Open Sans; display: inline-block; margin: 0px auto; text-align: center;}\n";
    htmlcode += "body{margin-top: 100px; display: flex; flex-direction: column; align-items: center;}\n";  /* Updated line */
    htmlcode += "h1 {color: #154c79; margin: 30px auto 30px;} h3 {color: #1e81b0; margin-bottom: 50px;}\n";
    htmlcode += ".button {display: block; width: 150px; background-color: #3498db; border: none; color: white; padding: 13px 30px; text-decoration: none; margin: 10px auto;}\n"; /* Added margin for better centering */
    htmlcode += ".button-on {background-color: #1e81b0;}\n";
    htmlcode += ".button-on:active {background-color: #15767b;}\n";
    htmlcode += ".button-off {background-color: #5e0f1a;}\n";
    htmlcode += ".button-off:active {background-color: #4b0c15;}\n";
    htmlcode += "p {font-size: 18px; color: #888; margin-bottom: 10px;}\n";
    htmlcode += "</style>\n";
    


    htmlcode += "</head>\n";
    htmlcode += "<body>\n";
    htmlcode += "<h1>ESP32 Web Server</h1>\n";
    htmlcode += "<h3>A simple demo using Access Point (AP) Mode</h3>\n";
    if (LEDStatus) {
        htmlcode += "<p>Blue LED Status: ON</p><p><a class=\"button button-off\" href=\"/ledoff\">Turn it OFF</a></p>\n";
    } else {
        htmlcode += "<p>Blue LED Status: OFF</p><p><a class=\"button button-on\" href=\"/ledon\">Turn it ON</a></p>\n";
    }
    htmlcode += "</body>\n";
    htmlcode += "</html>\n";
    return htmlcode;
}



