int ledIntegrado = 13; // LED integrado en Arduino UNO
int ledExterno   = 8;  // LED externo en pin 8

void setup() {
  pinMode(ledIntegrado, OUTPUT);
  pinMode(ledExterno, OUTPUT);

  // Configurar Timer1 en modo CTC
  noInterrupts();           // Deshabilitar interrupciones globales

  TCCR1A = 0;               // Modo normal
  TCCR1B = 0;               // Reset registros
  TCNT1  = 0;               // Contador en 0

  // Queremos una interrupción cada 500 ms (0.5 s)
  // Fórmula: OCR1A = (16e6) / (prescaler * frecuencia) - 1
  // F_CPU = 16 MHz, frecuencia = 2 Hz (500 ms), prescaler = 1024
  // OCR1A = 16e6 / (1024*2) - 1 = 7811
  OCR1A = 7811;

  TCCR1B |= (1 << WGM12);   // Modo CTC
  TCCR1B |= (1 << CS12) | (1 << CS10); // Prescaler 1024
  TIMSK1 |= (1 << OCIE1A);  // Habilitar interrupción en OCR1A

  interrupts();             // Habilitar interrupciones globales
}

void loop() {
  // El parpadeo lo maneja el timer
}

// ===== Interrupción de Timer1 cada 500 ms =====
ISR(TIMER1_COMPA_vect) {
  static bool estado = LOW;
  estado = !estado;

  digitalWrite(ledIntegrado, estado);
  digitalWrite(ledExterno, estado);
}