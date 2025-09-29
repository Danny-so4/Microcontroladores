#include <Wire.h>

void setup() {
  Wire.begin(8);               // Dirección del esclavo (0x08)
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
}

void loop() {}

void receiveEvent(int howMany) {
  while (Wire.available()) {
    int numero = Wire.read();
    Serial.print("Recibido en Arduino: ");
    Serial.println(numero);
  }
}
