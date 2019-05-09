#include "global.h" //Import Variables Globales
#include "src/wifi-connection/wifi-connet.h" //Import Conexion WIFI
#include "src/oximeter/oximeter.h" //Import Conexion WIFI
#include <HTTPClient.h>  // a definir
#include <ArduinoJson.h> // a definir

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //eligo velocidad de comunicacion con el dispocitivo
  connectWifi();
}

void loop() {
  // put your main code here, to run repeatedly:
      Serial.println(); //se deja una linea en la consola
      
      //Services REST
      HTTPClient http;
      http.begin("https://api.thingspeak.com/update.json");      //Specify request destination
      http.addHeader("Content-Type", "application/json");

      //JSON ARDUINO Version 5 No usar otra version
      StaticJsonBuffer<300> JSONbuffer;
      JsonObject& JSONencoder = JSONbuffer.createObject();
      char json[256];
      JSONencoder["api_key"] = API_KEY_MONIT;
      JSONencoder["field1"] = "10";
      JSONencoder["field2"] = "120";
      JSONencoder.printTo(json, sizeof(json));

      int httpCode = http.POST(json);   //Send the request
      String payload = http.getString();  //Get the response payload

      Serial.println(httpCode);   //Print HTTP return code
      Serial.println(payload); // se imprime lo enviado a la api

      //Finalizo conexión servicio
      http.end();
      Serial.println("Waiting for next loop…");
      delay(10000); //espero 10 segundos
}
