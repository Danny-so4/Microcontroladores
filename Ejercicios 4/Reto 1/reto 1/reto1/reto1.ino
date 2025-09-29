#include <Arduino_FreeRTOS.h>

// Definición de pines
const int led1 = 8;   // LED 1 en pin 8
const int led2 = 9;   // LED 2 en pin 9

// Prototipos de las tareas
void Tarea1(void *pvParameters);
void Tarea2(void *pvParameters);

void setup() {
  // Configurar pines como salida
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  // Crear tareas
  xTaskCreate(
    Tarea1,        // Función que implementa la tarea
    "LED1",        // Nombre descriptivo
    128,           // Tamaño de pila
    NULL,          // Parámetro
    1,             // Prioridad
    NULL           // Handle (no necesario aquí)
  );

  xTaskCreate(
    Tarea2,
    "LED2",
    128,
    NULL,
    1,
    NULL
  );

  // Inicia el planificador (lo maneja FreeRTOS automáticamente)
}

void loop() {
  // loop() queda vacío, todo lo maneja el RTOS
}

// ===== Definiciones de las tareas =====

// Tarea 1: LED enciende/apaga cada 500 ms
void Tarea1(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    digitalWrite(led1, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(led1, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// Tarea 2: LED enciende/apaga cada 1000 ms
void Tarea2(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    digitalWrite(led2, HIGH);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    digitalWrite(led2, LOW);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
