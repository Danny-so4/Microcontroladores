#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  Wire.requestFrom(0x10, 1); // Pide 1 byte
  if (Wire.available()) {
    int val = Wire.read();
    Serial.print("Maestro 1 recibe: ");
    Serial.println(val);
  }
  delay(2000);
}
