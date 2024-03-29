#include <HTTPClient.h>  // a definir
#include <ArduinoJson.h> // a definir
#include <Arduino.h>
#include "services-rest.h"

void setupServicesRESTJson(){}

void sendServicesRESTJson(String key, String value){
	 //Services REST
      HTTPClient http;
      http.begin(ENDPOINT);      //Specify request destination
      http.addHeader("Content-Type", "application/json");

      //JSON ARDUINO Version 5 No usar otra version
      StaticJsonBuffer<300> JSONbuffer;
      JsonObject& JSONencoder = JSONbuffer.createObject();
      char json[256];
      JSONencoder["api_key"] = API_KEY_MONIT;
      JSONencoder[key] = value;
      JSONencoder.printTo(json, sizeof(json));

      int httpCode = http.POST(json);   //Send the request
      String payload = http.getString();  //Get the response payload

      Serial.println(httpCode);   //Print HTTP return code
      Serial.println(payload); // se imprime lo enviado a la api

      //Finalizo conexión servicio
      http.end();
}