#include "global.h" //Import Variables Globales
#include "src/wifi-connection/wifi-connet.h" //Import Conexion WIFI
#include "src/oximeter/oximeter.h" //Import Conexion Oximeter
#include "src/temperature/temperature.h" //Import Conexion Temperature
#include "src/services-rest/services-rest.h" //Import Conexion services-rest-JSON
#include "src/button-action/button-action.h" //Import button ON or OFF
#include "src/util/date-util.h" //Import Conexion Temperature

void setup() {
  // put your setup code here, to run once:
  Serial.begin(VELOCITY_PORT); //eligo velocidad de comunicacion con el dispocitivo

  pinMode(BOTON, INPUT);  // CONFIGURAR PINES COMO ENTRADAS
  pinMode(PIN_LED, OUTPUT); // CONFIGURAR PIN DE LED COMO SALIDA

  setupButton(BOTON);
  connectWifi();
  setupZoneHours();
  setupTemperature();
  setupOximeter();
}

void loop() {
      // put your main code here, to run repeatedly:
      Serial.println(); //se deja una linea en la consola

      Serial.println("execute Temperature 1 thread");
      //execute Temperature 1 thread
      loopTemperature();

       Serial.println("execute Oximeter 2 thread");
      //execute Oximeter 2 thread
      loopOximeter();
}

void loopTemperature(){
      //si apreta boton ejecuto
    if (getStatusButton() == LOW) {
       setTemperaturaMax(0);
       updateTimeClient();
        while (getStatusButton() == LOW) {
          Serial.println(getStatusButton()); //muestro estado del boton

          digitalWrite(PIN_LED, HIGH); //prende el led
          startTemperature();
        }
        digitalWrite(PIN_LED, LOW);//apaga el led
        Serial.println(getTemperaturaMax()); //imprimo en consola la temperatura tomada
        connectWifi();
        sendServicesRESTJson(KEY_FIELD_TEMPERATURE, (String)getTemperaturaMax()); // envio el dato a la API
        Serial.println(); //se deja una linea en la consola
    }
}

void loopOximeter(){
  executeOximeter(); //Ejecuto el oximeter pendiente que hacer para el envio del dato a la API
  delay(1000); //espeo 1 segundo
}
