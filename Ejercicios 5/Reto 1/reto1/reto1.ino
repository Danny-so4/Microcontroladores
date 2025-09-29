#include <Wire.h>

void setup() {
  Wire.begin();         // Inicia I²C como maestro
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Escaneo I2C...");

  byte count = 0;

  for (byte address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("Dispositivo encontrado en 0x");
      Serial.println(address, HEX);
      count++;
    }
  }

  if (count == 0) Serial.println("No se encontraron dispositivos I2C");
  else Serial.println("Escaneo terminado");
}

void loop() {}
