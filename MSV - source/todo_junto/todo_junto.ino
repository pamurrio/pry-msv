#include <WiFi.h> //libreria WiFi
#include <Adafruit_MLX90614.h> //libreria sensor temperatura
#include <ArduinoJson.h> // a definir
#include <HTTPClient.h>  // a definir
#include <NTPClient.h> // a definir
#include <WiFiUdp.h> // a definir
#include "MAX30100_PulseOximeter.h"


#define REPORTING_PERIOD_MS     1000

// ASIGNACION DE PINES
const int boton = 23;
const int pinled = 2;
int estaon;
bool lei;
float temp = 0;

//---------------------VARIABLES GLOBALES-------------------------

const char* ssid     = "AndroidAP"; // nombre de la red WiFi
const char* password = "andres10"; // clave de la red WiFi
String apiKey = "I5T49S292QVA7O3A"; //clave de servicio Rest
int contconexion = 0;
String mac = "3C:71:BF:1D:42:F4"; // Mac Address modulo devKit
byte shaResult[32]; //el hash siempre va a tener 32 caracteres
const size_t payloadLength = 31;
String formattedDate;
String dayStamp; // Variables to save date and time
String timeStamp; // Variables to save date and time
String anio; // Variables to save date and time
String mes; // Variables to save date and time
String dia; // Variables to save date and time
String hora; // Variables to save date and time
String minutos; // Variables to save date and time
String segundos; // Variables to save date and time
Adafruit_MLX90614 mlx = Adafruit_MLX90614(); // se instancia el sensor de temperatura
WiFiServer server(80); // se conecta al puerto 80 de wifi
WiFiUDP ntpUDP; // Define NTP Client to get time
NTPClient timeClient(ntpUDP); // Define NTP Client to get time


//// oximetro
PulseOximeter pox;// intanciamos el oximetro
uint32_t tsLastReport = 0;
//// Callback (registered below) fired when a pulse is detected
////
//void onBeatDetected()
//{
//  Serial.println("Beat!");
//}

void setup()
{
  Serial.begin(9600); //eligo velocidad de comunicacion con el dispocitivo




  pinMode(boton, INPUT);  // CONFIGURAR PINES COMO ENTRADAS
  pinMode(pinled, OUTPUT); // CONFIGURAR PIN DE LED COMO SALIDA

  // Conexión WIFI
  WiFi.begin(ssid, password);
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


  timeClient.begin(); // Initialize a NTPClient to get time
  timeClient.setTimeOffset(-10800);  // Ajuste a Zona Horaria GMT-3


  mlx.begin(); // prende el sensor de temperatura
  //pox.begin(); // prende el oximetro y devuelve un booleano
  //
  //// oximetro
  //  // Initialize the PulseOximeter instance
  //  // Failures are generally due to an improper I2C wiring, missing power supply
  //  // or wrong target chip
  //  if (!pox.begin()) {
  //   Serial.println("FAILED");
  // } else {
  //    Serial.println("SUCCESS");
  //  }

  //pox.setOnBeatDetectedCallback(onBeatDetected);
}


//obtenemos fecha dia mes y hora y se formatea
String getDate() {
  // formattedDate viene con el siguiente formato: 2018-05-28T16:00:13Z
  formattedDate = timeClient.getFormattedDate();

  // necesitamos extraer los distintos datos para reformatearl a YYmmDDhhMMss
  // Extract date
  anio = formattedDate.substring(0, 4);
  mes = formattedDate.substring(5, 7);
  dia = formattedDate.substring(8, 10);
  hora = formattedDate.substring(11, 13);
  minutos = formattedDate.substring(14, 16);
  segundos = formattedDate.substring(17, 19);

  dayStamp = anio + mes + dia; // dia anio y mes formateado
  timeStamp = hora + minutos + segundos; // hora formateada

  return dayStamp + timeStamp; //fecha completa
}


void loop()
{
  lei = false;
  estaon = digitalRead(boton);
  Serial.println(estaon);// mostramos estado del boton

  //si apreta boton ejecuto
  if (estaon == LOW) {

    while (!timeClient.update()) {
      timeClient.forceUpdate();
    }
    Serial.println(mac + getDate()); // mostramos concateno la mac del dispositivo con la fecha y hora

    while (digitalRead(boton) == LOW) {
      Serial.println(estaon); //muestro estado del boton

      digitalWrite(pinled, HIGH); //prende el led
      Serial.print("temperatura = ");
      Serial.print(mlx.readObjectTempC());//leo temperatura
      Serial.println("*C");
      Serial.println();
      if (temp < mlx.readObjectTempC()) {  //tomo la temperatura mas alta registrada
        temp = mlx.readObjectTempC();
      }

    }
    digitalWrite(pinled, LOW);//apaga el led
    Serial.println(temp); //imprimo en consola la temperatura tomada

    Serial.println(); //se deja una linea en la consola

    if (WiFi.status() == WL_CONNECTED) { //si estamos conectados enviamos mensaje sino, se pierde la info

      //Services REST
      HTTPClient http;
      http.begin("https://api.thingspeak.com/update.json");      //Specify request destination
      http.addHeader("Content-Type", "application/json");

      //JSON ARDUINO Version 5 No usar otra version
      StaticJsonBuffer<300> JSONbuffer;
      JsonObject& JSONencoder = JSONbuffer.createObject();
      char json[256];
      JSONencoder["api_key"] = apiKey;
      JSONencoder["field1"] = (String)temp;
      JSONencoder["field2"] = mac + getDate();
      //JSONencoder["field3"] = luminosidad;
      JSONencoder.printTo(json, sizeof(json));

      int httpCode = http.POST(json);   //Send the request
      String payload = http.getString();  //Get the response payload

      Serial.println(httpCode);   //Print HTTP return code
      Serial.println(payload); // se imprime lo enviado a la api

      //Finalizo conexión servicio
      http.end();
      Serial.println("Waiting for next loop…");

    }
    else {
      WiFi.begin(ssid, password);//vuelvo a tratar de conectarme al wifi
    }
  }
  temp = 0;
  if (lei) { //si leo temperatura espero 10 segundo q es lo que tarda el serv externo en recibir la llamada antes de poder tomar la proxima medicion
    //Espero 10 seg
    delay(10000); //espero 10 segundos
  } else {

    // ejecuto oximetro
    //  // Make sure to call update as fast as possible
    //  pox.update();
    //
    //  // Asynchronously dump heart rate and oxidation levels to the serial
    //  // For both, a value of 0 means "invalid"
    //  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    //    Serial.print("Heart rate:");
    //    Serial.print(pox.getHeartRate());
    //    Serial.print("bpm / SpO2:");
    //    Serial.print(pox.getSpO2());
    //    Serial.println("%");
    //
    //    tsLastReport = millis();
    //  }
    delay(1000); //espeo 1 segundo
  }
}
