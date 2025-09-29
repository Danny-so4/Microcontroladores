#include <Arduino.h>

// Pines de los LEDs
#define LED1 2   // LED conectado al pin 2
#define LED2 4   // LED conectado al pin 4

// Prototipos de las tareas
void TaskBlink500(void *pvParameters);
void TaskBlink1000(void *pvParameters);

void setup() {
  // Configurar pines como salida
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  // Crear tareas
  xTaskCreate(
    TaskBlink500,      // Función de la tarea
    "Blink500",        // Nombre de la tarea (para depuración)
    1000,              // Tamaño del stack (palabras, no bytes)
    NULL,              // Parámetro para la tarea
    1,                 // Prioridad de la tarea
    NULL               // Handle de la tarea
  );

  xTaskCreate(
    TaskBlink1000,
    "Blink1000",
    1000,
    NULL,
    1,
    NULL
  );
}

void loop() {
  // El loop queda vacío, RTOS se encarga de la multitarea
}

// --- Definición de tareas ---
void TaskBlink500(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    digitalWrite(LED1, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);  // Espera 500 ms
    digitalWrite(LED1, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void TaskBlink1000(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    digitalWrite(LED2, HIGH);
    vTaskDelay(1000 / portTICK_PERIOD_MS); // Espera 1000 ms
    digitalWrite(LED2, LOW);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
