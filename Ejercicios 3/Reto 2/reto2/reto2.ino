#include <EEPROM.h>

// Dirección en la EEPROM donde guardaremos la configuración
int direccion = 0;

// Variable para guardar la opción seleccionada (0 = lenta, 1 = media, 2 = rápida)
int opcion = 0;

// Pin del LED (LED externo en pin 9)
const int ledPin = 9;

// Intervalos de parpadeo en milisegundos
const unsigned long velocidades[] = {1000, 500, 200}; // lenta, media, rápida

// Variables para el parpadeo
unsigned long previoMillis = 0;
bool ledEstado = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);

  // Leer configuración almacenada en EEPROM
  EEPROM.get(direccion, opcion);

  // Validar que la opción leída sea válida (0, 1 o 2). Si no, resetear a 0.
  if (opcion < 0 || opcion > 2) {
    opcion = 0;
  }

  Serial.println("=== MENU DE CONFIGURACION ===");
  Serial.println("0: Velocidad lenta (1s)");
  Serial.println("1: Velocidad media (0.5s)");
  Serial.println("2: Velocidad rapida (0.2s)");
  Serial.println("Ingrese su opcion (0-2):");

  Serial.print("Configuracion actual: ");
  Serial.println(opcion);
}

void loop() {
  // --- Verificar si hay datos en el puerto serie ---
  if (Serial.available() > 0) {
    char entrada = Serial.read();
    if (entrada >= '0' && entrada <= '2') {
      opcion = entrada - '0';  // Convertir char a número (0-2)

      // Guardar en EEPROM
      EEPROM.put(direccion, opcion);

      Serial.print("Nueva configuracion guardada: ");
      Serial.println(opcion);
    }
  }

  // --- Lógica de parpadeo usando la configuración ---
  unsigned long intervalo = velocidades[opcion];
  unsigned long actualMillis = millis();

  if (actualMillis - previoMillis >= intervalo) {
    previoMillis = actualMillis;
    ledEstado = !ledEstado;
    digitalWrite(ledPin, ledEstado);
  }
}