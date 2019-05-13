#include <Arduino.h>
// ASIGNACION DE PINES
const int boton;

void setupButton(int btnAction){
	boton = btnAction;
	pinMode(boton, INPUT);  // CONFIGURAR PINES COMO ENTRADAS
}
int getStatusButton(){
 return digitalRead(boton);
}