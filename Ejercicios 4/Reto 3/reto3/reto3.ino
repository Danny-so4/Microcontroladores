#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

// Mutex para proteger el recurso compartido (Serial)
SemaphoreHandle_t xMutex;

// Prototipos de tareas
void TaskWrite1(void *pvParameters);
void TaskWrite2(void *pvParameters);

void setup() {
  Serial.begin(9600);
  while (!Serial);  // Esperar a que el monitor esté listo (opcional)

  // Crear mutex
  xMutex = xSemaphoreCreateMutex();

  if (xMutex != NULL) {
    // Crear tareas con stack más pequeño
    xTaskCreate(TaskWrite1, "Writer1", 128, NULL, 1, NULL);
    xTaskCreate(TaskWrite2, "Writer2", 128, NULL, 1, NULL);
  } else {
    Serial.println("Error: No se pudo crear el mutex");
  }
}

void loop() {
  // No se usa, FreeRTOS maneja las tareas
}

// --- Tarea 1 ---
void TaskWrite1(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    if (xSemaphoreTake(xMutex, (TickType_t) 10) == pdTRUE) {
      Serial.println("Tarea 1 escribiendo en el recurso...");
      vTaskDelay(pdMS_TO_TICKS(500)); // Simular que tarda escribiendo
      Serial.println("Tarea 1 terminó de escribir.");
      xSemaphoreGive(xMutex);
    }
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

// --- Tarea 2 ---
void TaskWrite2(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    if (xSemaphoreTake(xMutex, (TickType_t) 10) == pdTRUE) {
      Serial.println("Tarea 2 escribiendo en el recurso...");
      vTaskDelay(pdMS_TO_TICKS(300)); // Simular que tarda menos
      Serial.println("Tarea 2 terminó de escribir.");
      xSemaphoreGive(xMutex);
    }
    vTaskDelay(pdMS_TO_TICKS(800));
  }
}
