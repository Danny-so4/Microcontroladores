#include <Wire.h>

void setup() {
  Wire.begin(); // Inicia como maestro
  Serial.begin(9600);
}

void loop() {
  int numero = random(0, 100); // Número aleatorio
  Wire.beginTransmission(8);   // Dirección del esclavo
  Wire.write(numero);          // Envía un byte
  Wire.endTransmission();

  Serial.print("Enviado: ");
  Serial.println(numero);

  delay(1000);
}
