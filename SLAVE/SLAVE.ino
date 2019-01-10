#include <ESP8266WiFi.h> 
extern "C" {
  #include <espnow.h>
}
 
//***ESTRUCTURA DE LOS DATOS TRANSMITIDOS MAESTRO/ESCLAVO***//
//Se de establecer IGUAL en el par maestro
    struct DATA_STRUCT {
    uint16_t X;
    uint16_t Y;
    uint16_t Ax;
    uint16_t Ay;
    uint16_t Az;
    uint8_t Buttons;
};
 
 
void setup() {
 
  //***INICIALIZACIÓN DEL PUERTO SERIE***//
  Serial.begin(115200); Serial.println();
 
  //***INICIALIZACIÓN DEL PROTOCOLO ESP-NOW***//   
  if (esp_now_init()!=0) {
    Serial.println("Protocolo ESP-NOW no inicializado...");
    ESP.restart();
    delay(1);
  }
 
  //***DECLARACIÓN DEL PAPEL DEL DISPOSITIVO ESP EN LA COMUNICACIÓN***//
  //0=OCIOSO, 1=MAESTRO, 2=ESCLAVO y 3=MAESTRO+ESCLAVO 
  esp_now_set_self_role(2);   
 
}
 
void loop() {
 
  //***RECEPCIÓN DE LA COMUNICACIÓN ESP-NOW***//
   esp_now_register_recv_cb([](uint8_t *mac, uint8_t *data, uint8_t len) {
 
    char MACmaster[6];
    sprintf(MACmaster, "%02X:%02X:%02X:%02X:%02X:%02X",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
    Serial.print("Read from ESP MAC: "); Serial.println(MACmaster);
 
    DATA_STRUCT DS;
    memcpy(&DS, data, sizeof(DS));
 
    Serial.print("X: "); Serial.println(DS.X);
    Serial.print("Y: "); Serial.println(DS.Y);
    Serial.print("Ax: "); Serial.println(DS.Ax);
    Serial.print("Ay: "); Serial.println(DS.Ay);
    Serial.print("Az: "); Serial.println(DS.Az);
    Serial.print("C: "); Serial.println(DS.Buttons%10);
    Serial.print("Z: "); Serial.println(DS.Buttons/10);
  });
 
}
