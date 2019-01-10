#include <ESP8266WiFi.h>  
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA);
  Serial.println();
  Serial.print("MAC Access Point: ");
  Serial.println(WiFi.softAPmacAddress()); 
  Serial.print("MAC Station: ");
  Serial.println(WiFi.macAddress()); 
}

void loop() {}
