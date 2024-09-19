
/*
 * install 
 * DHT sensor library by Adafruit, version 1.4.6, 
 */

#include <DHT.h>
DHT dht(4, DHT22);

void setup() {
  dht.begin();
  delay(2000);
Serial.begin(115200);
}

void loop() {
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print(" C ");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" % ");
  delay(2000);

}

/*
load:0x40080400,len:4
load:0x40080404,len:3356
entry 0x4008059c
Temp: 26.40 C Humidity: 19.70 % 
Temp: 26.40 C Humidity: 19.80 % 
Temp: 26.40 C Humidity: 19.80 % 
Temp: 26.40 C Humidity: 19.80 % 
Temp: 26.40 C Humidity: 19.80 % 
Temp: 26.40 C Humidity: 19.80 % 
Temp: 26.40 C Humidity: 19.80 % 
Temp: 26.40 C Humidity: 19.80 % 
Temp: 26.40 C Humidity: 19.90 % 
Temp: 26.40 C Humidity: 19.80 % 
Temp: 26.40 C Humidity: 24.00 % 
Temp: 26.40 C Humidity: 24.00 % 
Temp: 26.40 C Humidity: 24.10 % 
Temp: 26.40 C Humidity: 24.10 % 
Temp: 26.40 C Humidity: 24.00 % 
Temp: 26.40 C Humidity: 24.10 % 
Temp: 26.40 C Humidity: 24.00 % 
*/
