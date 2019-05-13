// ASIGNACION DE PINES
const int boton = 23;

void setupButton(){
	pinMode(boton, INPUT);  // CONFIGURAR PINES COMO ENTRADAS
}
int getStatusButton(){
 return digitalRead(boton);
}