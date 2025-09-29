int leds[4] = {3,4,5,6};
int boton = 2;
volatile int numero = 0;   // "volatile" porque lo modifica la interrupción

void setup() {
  for (int i = 0; i < 4; i++) pinMode(leds[i], OUTPUT);
  pinMode(boton, INPUT_PULLUP);

  // Configurar interrupción externa en pin 2 (INT0)
  attachInterrupt(digitalPinToInterrupt(boton), incrementarContador, FALLING);
}

void loop() {
  // Mostrar el valor actual en binario
  mostrarBinario(numero);
}

// ===== Función para mostrar en binario =====
void mostrarBinario(int n) {
  for (int i = 0; i < 4; i++) {
    digitalWrite(leds[i], (n >> i) & 1);
  }
}

// ===== Rutina de interrupción =====
void incrementarContador() {
  numero = (numero + 1) % 16;  // Contador de 0 a 15
}