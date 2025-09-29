const int pinSensor = 2;   // Entrada de pulsos (INT0)
const int led = 8;         // LED indicador

volatile unsigned long contadorPulsos = 0;
volatile bool flagMedir = false;

void setup() {
  pinMode(pinSensor, INPUT_PULLUP);
  pinMode(led, OUTPUT);

  Serial.begin(9600);

  attachInterrupt(digitalPinToInterrupt(pinSensor), contarPulsos, FALLING);

  // Configurar Timer1 en modo CTC (1s)
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  OCR1A = 15624;                       // 1 Hz con prescaler 1024
  TCCR1B |= (1 << WGM12);              // CTC
  TCCR1B |= (1 << CS12) | (1 << CS10); // prescaler 1024
  TIMSK1 |= (1 << OCIE1A);             // habilita interrupción
  interrupts();
}

void loop() {
  if (flagMedir) {
    noInterrupts();
    unsigned long pulsos = contadorPulsos;
    contadorPulsos = 0;
    interrupts();

    Serial.print("Frecuencia: ");
    Serial.print(pulsos);
    Serial.println(" Hz");

    flagMedir = false;
  }
}

void contarPulsos() {
  contadorPulsos++;
  digitalWrite(led, !digitalRead(led));
}

// 🚨 IMPORTANTE: deja el ISR al final
ISR(TIMER1_COMPA_vect) {
  flagMedir = true;
}