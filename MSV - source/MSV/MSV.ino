#include "global.h" //Import Variables Globales
#include "src/wifi-connection/wifi-connet.h" //Import Conexion WIFI
#include "src/oximeter/oximeter.h" //Import Conexion Oximeter
#include "src/services-rest/services-rest.h" //Import Conexion services-rest-JSON

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //eligo velocidad de comunicacion con el dispocitivo
  connectWifi();
  setupTemperature();
}

void loop() {
      // put your main code here, to run repeatedly:
      Serial.println(); //se deja una linea en la consola
      sendServicesRESTJson("field1", "10");
      Serial.println("Waiting for next loop…");
      delay(10000); //espero 10 segundos
}
