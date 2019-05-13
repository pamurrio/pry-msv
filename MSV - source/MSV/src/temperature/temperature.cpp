#include <Adafruit_MLX90614.h> 

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
float temp = 0;

void setupTemperature(){
	 mlx.begin();
}

void startTemperature(){
	Serial.print("temperatura = ");
	Serial.print(mlx.readObjectTempC());//leo temperatura
	Serial.println("*C");
	Serial.println();
	if (temp < mlx.readObjectTempC()) {  //tomo la temperatura mas alta registrada
		temp = mlx.readObjectTempC();
	}
}

float getTemperaturaMax(){
	return temp;
}