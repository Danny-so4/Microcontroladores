#include <Wire.h>

#define SLAVE_ADDR 0x10

void setup() {
  Wire.begin(SLAVE_ADDR);
  Wire.onRequest(requestEvent);
}

void loop() {
  // Aquí pondrías código para leer un sensor real
}

void requestEvent() {
  int value = random(20, 40); // Simula dato
  Wire.write(value);
}
