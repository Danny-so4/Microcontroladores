#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include "DHT.h"

// --- Configuración DHT11 ---
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// --- LED de alarma ---
#define LED 3

// --- Cola para comunicación ---
QueueHandle_t xQueue;

// Prototipos
void TaskReadSensor(void *pvParameters);
void TaskProcessData(void *pvParameters);

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(LED, OUTPUT);

  // Crear la cola (10 enteros de 4 bytes c/u)
  xQueue = xQueueCreate(10, sizeof(int));

  if (xQueue != NULL) {
    // Crear tareas
    xTaskCreate(TaskReadSensor, "ReadSensor", 1000, NULL, 1, NULL);
    xTaskCreate(TaskProcessData, "ProcessData", 1000, NULL, 1, NULL);
  } else {
    Serial.println("Error: No se pudo crear la cola");
  }
}

void loop() {
  // RTOS se encarga, loop vacío
}

// --- Tarea de lectura de sensor ---
void TaskReadSensor(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    int temp = (int)dht.readTemperature();

    if (!isnan(temp)) {
      // Enviar valor a la cola
      if (xQueueSend(xQueue, &temp, portMAX_DELAY) == pdPASS) {
        Serial.print("Sensor -> enviado: ");
        Serial.println(temp);
      }
    }
    vTaskDelay(2000 / portTICK_PERIOD_MS); // cada 2 segundos
  }
}

// --- Tarea de procesamiento ---
void TaskProcessData(void *pvParameters) {
  (void) pvParameters;
  int receivedValue;

  for (;;) {
    // Esperar hasta recibir un dato de la cola
    if (xQueueReceive(xQueue, &receivedValue, portMAX_DELAY) == pdPASS) {
      Serial.print("Procesador <- recibido: ");
      Serial.println(receivedValue);

      // Umbral de temperatura
      if (receivedValue > 28) {
        digitalWrite(LED, HIGH);
        Serial.println("⚠️ Alerta: Temperatura alta!");
      } else {
        digitalWrite(LED, LOW);
      }
    }
  }
}
