#include <EEPROM.h>  // Librería para usar la EEPROM

int direccion = 0;   // Dirección en la EEPROM donde guardaremos el contador
int contador = 0;    // Variable para guardar el número de encendidos

void setup() {
  Serial.begin(9600);

  // Leer el valor actual de la EEPROM
  contador = EEPROM.read(direccion);

  // Incrementar en 1 cada vez que se enciende el Arduino
  contador++;

  // Guardar el nuevo valor en la EEPROM
  EEPROM.write(direccion, contador);

  // Mostrar el número de encendidos por el puerto serie
  Serial.print("Numero de encendidos: ");
  Serial.println(contador);
}

void loop() {
  // El loop queda vacío porque solo queremos contar encendidos al inicio
}