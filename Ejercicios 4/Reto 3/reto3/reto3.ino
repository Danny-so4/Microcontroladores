#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

// Declarar el mutex
SemaphoreHandle_t xMutex;

// Prototipos
void TaskWrite1(void *pvParameters);
void TaskWrite2(void *pvParameters);

void setup() {
  Serial.begin(9600);

  // Crear mutex
  xMutex = xSemaphoreCreateMutex();

  if (xMutex != NULL) {
    // Crear tareas
    xTaskCreate(TaskWrite1, "Writer1", 1000, NULL, 1, NULL);
    xTaskCreate(TaskWrite2, "Writer2", 1000, NULL, 1, NULL);
  } else {
    Serial.println("Error: No se pudo crear el mutex");
  }
}

void loop() {
  // RTOS se encarga, loop vacío
}

// --- Tareas ---
void TaskWrite1(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    // Intentar tomar el mutex
    if (xSemaphoreTake(xMutex, (TickType_t) 10) == pdTRUE) {
      Serial.println("Tarea 1 escribiendo en el recurso...");
      vTaskDelay(500 / portTICK_PERIOD_MS); // Simular que tarda escribiendo
      Serial.println("Tarea 1 terminó de escribir.");
      // Liberar el mutex
      xSemaphoreGive(xMutex);
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS); // Esperar antes de volver a intentar
  }
}

void TaskWrite2(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    if (xSemaphoreTake(xMutex, (TickType_t) 10) == pdTRUE) {
      Serial.println("Tarea 2 escribiendo en el recurso...");
      vTaskDelay(300 / portTICK_PERIOD_MS); // Simular que tarda menos
      Serial.println("Tarea 2 terminó de escribir.");
      xSemaphoreGive(xMutex);
    }
    vTaskDelay(800 / portTICK_PERIOD_MS);
  }
}
