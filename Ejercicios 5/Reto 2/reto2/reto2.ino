#include <Arduino.h>

// Pines
#define LED_LOW     2   // LED de baja prioridad
#define LED_BUTTON  3   // LED controlado por el botón (alta prioridad)
#define BUTTON      4   // Botón
#define POT         A0  // Potenciómetro

// Prototipos
void TaskBlinkLow(void *pvParameters);
void TaskReadPot(void *pvParameters);
void TaskButtonHigh(void *pvParameters);

void setup() {
  Serial.begin(9600);

  // Configuración de pines
  pinMode(LED_LOW, OUTPUT);
  pinMode(LED_BUTTON, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP); // Botón con resistencia interna

  // Crear tareas
  // --- Cambia la prioridad (último argumento) para probar ---
  xTaskCreate(TaskBlinkLow, "BlinkLow", 1000, NULL, 1, NULL);   // Baja prioridad
  xTaskCreate(TaskReadPot,  "ReadPot",  1000, NULL, 2, NULL);   // Media prioridad
  xTaskCreate(TaskButtonHigh,"Button",  1000, NULL, 3, NULL);   // Alta prioridad
}

void loop() {
  // El loop queda vacío porque FreeRTOS maneja las tareas
}

// --- Tareas ---
void TaskBlinkLow(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    digitalWrite(LED_LOW, HIGH);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    digitalWrite(LED_LOW, LOW);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void TaskReadPot(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    int value = analogRead(POT);
    Serial.print("Potenciometro: ");
    Serial.println(value);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void TaskButtonHigh(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    if (digitalRead(BUTTON) == LOW) { // Pulsado
      digitalWrite(LED_BUTTON, HIGH);
    } else {
      digitalWrite(LED_BUTTON, LOW);
    }
    // No usamos delay aquí, revisa continuamente
    vTaskDelay(1); // Cede un poquito de tiempo al resto
  }
}
