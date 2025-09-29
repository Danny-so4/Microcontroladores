const int sensor = 2;   // PIR o pulsador (INT0)
const int stopBtn = 3;  // Botón para apagar alarma (INT1)
const int buzzer = 9;   // Buzzer (usaremos Timer2 para generar tono)

volatile bool alarmaActiva = false;   // Estado de la alarma
volatile bool toggleBuzzer = false;   // Para alternar el pin en la ISR

void setup() {
  pinMode(sensor, INPUT_PULLUP);   // Pulsador o PIR
  pinMode(stopBtn, INPUT_PULLUP);  // Botón de apagado
  pinMode(buzzer, OUTPUT);

  // Interrupciones externas
  attachInterrupt(digitalPinToInterrupt(sensor), encenderAlarma, FALLING);
  attachInterrupt(digitalPinToInterrupt(stopBtn), apagarAlarma, FALLING);

  // Configurar Timer2 para generar interrupción periódica
  noInterrupts();
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2  = 0;

  OCR2A = 124;                      // Valor comparador
  TCCR2A |= (1 << WGM21);           // Modo CTC
  TCCR2B |= (1 << CS22);            // Prescaler 64
  TIMSK2 |= (1 << OCIE2A);          // Habilitar interrupción
  interrupts();

  Serial.begin(9600);
}

void loop() {
  // El control real lo hacen las ISR
}

// === ISR externas ===
void encenderAlarma() {
  alarmaActiva = true;
  Serial.println("🚨 Alarma ACTIVADA 🚨");
}

void apagarAlarma() {
  alarmaActiva = false;
  digitalWrite(buzzer, LOW); // Apaga el buzzer
  Serial.println("✅ Alarma DESACTIVADA");
}

// === ISR de Timer2 ===
ISR(TIMER2_COMPA_vect) {
  if (alarmaActiva) {
    toggleBuzzer = !toggleBuzzer;
    digitalWrite(buzzer, toggleBuzzer);
  }
}