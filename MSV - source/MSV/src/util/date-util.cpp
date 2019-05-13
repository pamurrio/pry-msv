#include <NTPClient.h> // a definir
#include <WiFiUdp.h> // a definir

WiFiUDP ntpUDP; // Define NTP Client to get time
NTPClient timeClient(ntpUDP); // Define NTP Client to get time

//obtenemos fecha dia mes y hora y se formatea
String getDate() {
  String formattedDate;
  String dayStamp; // Variables to save date and time
  String timeStamp; // Variables to save date and time
  String anio; // Variables to save date and time
  String mes; // Variables to save date and time
  String dia; // Variables to save date and time
  String hora; // Variables to save date and time
  String minutos; // Variables to save date and time
  String segundos; // Variables to save date and time
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

void setupZoneHours(){
  timeClient.begin(); // Initialize a NTPClient to get time
  timeClient.setTimeOffset(-10800);  // Ajuste a Zona Horaria GMT-3
}

void updateTimeClient(){
  while (!timeClient.update()) {
      timeClient.forceUpdate();
    }
  Serial.println(mac + getDate());
}