#include <Wire.h>

#define I2C_SDA 21   // Pines I²C del ESP32 (por defecto SDA=21, SCL=22)
#define I2C_SCL 22

void setup() {
  Wire.begin(I2C_SDA, I2C_SCL); // Inicia como maestro en ESP32
  Serial.begin(115200);
}

void loop() {
  int numero = random(0, 100);

  Wire.beginTransmission(8);  // Dirección del esclavo (Arduino)
  Wire.write(numero);
  Wire.endTransmission();

  Serial.print("Enviado desde ESP32: ");
  Serial.println(numero);

  delay(1000);
}
