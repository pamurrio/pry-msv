#include <Adafruit_MLX90614.h> //libreria sensor temperatura
#include "MAX30100_PulseOximeter.h" //libreria sensor oximetro
#include <Wire.h>

#define REPORTING_PERIOD_MS     1000

// ASIGNACION DE PINES
const int boton = 23;
const int pinled = 2;
int estaon;
bool lei;
float temp = 0;



//// oximetro

PulseOximeter pox;// intanciamos el oximetro
Adafruit_MLX90614 mlx;
uint32_t tsLastReport = 0;
//// Callback (registered below) fired when a pulse is detected
////
void onBeatDetected()
{
  Serial.println("Beat!");
}


// se instancia el sensor de temperatura

void setup()
{

  Wire.begin(21, 22);
  Serial.begin(9600); //eligo velocidad de comunicacion con el dispocitivo

  pinMode(boton, INPUT);  // CONFIGURAR PINES COMO ENTRADAS
  pinMode(pinled, OUTPUT); // CONFIGURAR PIN DE LED COMO SALIDA



  // Initialize the PulseOximeter instance
  // Failures are generally due to an improper I2C wiring, missing power supply
  // or wrong target chip
  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;);
  } else {
    Serial.println("SUCCESS");
  }
  mlx = Adafruit_MLX90614();
  mlx.begin(); // prende el sensor de temperatura

  // The default current for the IR LED is 50mA and it could be changed
  //   by uncommenting the following line. Check MAX30100_Registers.h for all the
  //   available options.
  // pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

  // Register a callback for the beat detection
  pox.setOnBeatDetectedCallback(onBeatDetected);
}




void loop()
{
  lei = false;
  estaon = digitalRead(boton);

  // ejecuto oximetro
  
  pox.update();
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    Serial.print("Heart rate:");
    Serial.print(pox.getHeartRate());
    Serial.print("bpm / SpO2:");
    Serial.print(pox.getSpO2());
    Serial.println("%");
    //
    tsLastReport = millis();
  }
  //si apreta boton ejecuto
  if (estaon == LOW) {

    while (digitalRead(boton) == LOW) {
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
delay(10000);
  }
  temp = 0;
}
