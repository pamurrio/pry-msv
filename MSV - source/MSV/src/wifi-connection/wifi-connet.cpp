#include <WiFi.h> //libreria WiFi
#include <Arduino.h>
#include "wifi-connet.h"

void connectWifi()
{
  int contconexion = 0;
   // Conexión WIFI
  WiFi.begin(SSID_WIFI, PASS_WIFI);
  //Cuenta hasta 50 si no se puede conectar lo cancela
  while (WiFi.status() != WL_CONNECTED and contconexion < 50) {
    ++contconexion;
    delay(500);
    Serial.print(".");
  }
  if (contconexion < 50) {

    // Serial.println("");
    Serial.println("WiFi conectado");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("Error de conexion");
  }  
}
