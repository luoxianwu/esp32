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
    
    htmlcode += "</head>\n";
    htmlcode += "<body>\n";
    htmlcode += "<h1>ESP32 Web Server</h1>\n";
    htmlcode += "<h3>A simple demo using Access Point (AP) Mode</h3>\n";
    if (LEDStatus) {
        htmlcode += "<p>Blue LED Status: ON</p><p><a href=\"/ledoff\">Turn it OFF</a></p>\n";
    } else {
        htmlcode += "<p>Blue LED Status: OFF</p><p><a href=\"/ledon\">Turn it ON</a></p>\n";
    }
    htmlcode += "</body>\n";
    htmlcode += "</html>\n";
    return htmlcode;
}



