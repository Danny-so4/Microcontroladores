#include <Wire.h>

void setup() {
  Wire.begin(8);               // Dirección del esclavo
  Wire.onReceive(receiveEvent); // Callback al recibir datos
  Serial.begin(9600);
}

void loop() {}

void receiveEvent(int howMany) {
  while (Wire.available()) {
    int numero = Wire.read();
    Serial.print("Recibido: ");
    Serial.println(numero);
  }
}
