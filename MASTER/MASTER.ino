 
#include <ESP8266WiFi.h>
extern "C" {
  #include <espnow.h>
}
#include <Wire.h>
#include "ArduinoNunchuk.h"

#define BAUDRATE 115200

ArduinoNunchuk nunchuk = ArduinoNunchuk();

unsigned long previousMillis = 0;
const long interval = 1000; 
 
struct DATA_STRUCT {
    uint16_t X;
    uint16_t Y;
    uint16_t Ax;
    uint16_t Ay;
    uint16_t Az;
    uint8_t Buttons;
};
 
void setup() {
 
  Serial.begin(BAUDRATE); 
  Serial.println();
  nunchuk.init();
  
  if (esp_now_init()!=0) {
    Serial.println("ESP-Now init failed");
    ESP.restart();
    delay(1);
  }
 
  //0=IDLE, 1=MASTER, 2=SLAVE y 3=MASTER+SLAVE 
  esp_now_set_self_role(1);
  
  // SLAVE STA MAC
  uint8_t mac_addr[6] = {0x84, 0xF3, 0xEB, 0x0C, 0x35, 0xE3};    
  uint8_t role=2;  
  uint8_t channel=3;
  uint8_t key[0]={};
  //uint8_t key[16] = {0,255,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
  uint8_t key_len=sizeof(key);
  Serial.print("KEY SIZE:  "); Serial.println(key_len);
  esp_now_add_peer(mac_addr,role,channel,key,key_len);
}
 
void loop() {

  unsigned long currentMillis = millis();
  DATA_STRUCT DS;
  
  //***GET DATA to Send***//
  nunchuk.update();
  
  DS.X = nunchuk.analogX;
  //Serial.print("X: "); Serial.println(DS.X);
  DS.Y = nunchuk.analogY;
  //Serial.print("Y: "); Serial.println(DS.Y);
  DS.Ax = nunchuk.accelX;
  //Serial.print("Ax: "); Serial.println(DS.Ax);
  DS.Ay = nunchuk.accelY;
  //Serial.print("Ay: "); Serial.println(DS.Ay);
  DS.Az = nunchuk.accelZ;
  //Serial.print("Az: "); Serial.println(DS.Az);
  DS.Buttons = nunchuk.zButton*10+nunchuk.cButton;
  //Serial.print("Buttons: "); Serial.println(DS.Buttons);
  
  delay(20);
 
  //***Data to Send***//
    //uint8_t *da=NULL;   //NULL if would send to all ESP paired
    uint8_t da[6] = {0x84, 0xF3, 0xEB, 0x0C, 0x35, 0xE3};
    uint8_t data[sizeof(DS)]; memcpy(data, &DS, sizeof(DS));
    uint8_t len = sizeof(data);
  esp_now_send(da, data, len);
 
  delay(2); //Up if data is lost
 
  //***Check if data is received***//
  esp_now_register_send_cb([](uint8_t* mac, uint8_t status) {
    char MACslave[6];
    sprintf(MACslave,"%02X:%02X:%02X:%02X:%02X:%02X",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
    if (status) {
      Serial.println("ERROR");}
    else {
      Serial.print("Sent to ESP MAC: "); Serial.println(MACslave);}
  });
}
