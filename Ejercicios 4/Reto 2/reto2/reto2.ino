// ESP32 + FreeRTOS version
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

// Pines (ajustados para ESP32)
#define LED_LOW     2    // GPIO2
#define LED_BUTTON  4    // GPIO4 (evitamos GPIO3 RX)
#define BUTTON      15   // GPIO15 (con INPUT_PULLUP)
#define POT         34   // ADC1_6 (GPIO34) -> pin analógico

// Prototipos
void TaskBlinkLow(void *pvParameters);
void TaskReadPot(void *pvParameters);
void TaskButtonHigh(void *pvParameters);

void setup() {
  Serial.begin(115200);

  pinMode(LED_LOW, OUTPUT);
  pinMode(LED_BUTTON, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP); // pulsador a GND cuando se presiona

  // Crear tareas (stack size reducido a 2048 bytes / prioridad según tu diseño)
  xTaskCreate(TaskBlinkLow, "BlinkLow", 2048, NULL, 1, NULL);   // Baja prioridad
  xTaskCreate(TaskReadPot,  "ReadPot",  4096, NULL, 2, NULL);   // Media prioridad
  xTaskCreate(TaskButtonHigh,"Button",  2048, NULL, 3, NULL);   // Alta prioridad
}

void loop() {
  // vacío, FreeRTOS gestiona las tareas
}

// --- Tareas ---
void TaskBlinkLow(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    digitalWrite(LED_LOW, HIGH);
    vTaskDelay(pdMS_TO_TICKS(1000));
    digitalWrite(LED_LOW, LOW);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void TaskReadPot(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    int value = analogRead(POT); // 0..4095 en ESP32 dependiendo de la atenuación
    Serial.print("Potenciometro: ");
    Serial.println(value);
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void TaskButtonHigh(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    if (digitalRead(BUTTON) == LOW) { // pulsado (INPUT_PULLUP)
      digitalWrite(LED_BUTTON, HIGH);
    } else {
      digitalWrite(LED_BUTTON, LOW);
    }
    vTaskDelay(pdMS_TO_TICKS(1)); // cede muy poco tiempo
  }
}
