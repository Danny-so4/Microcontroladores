int ledPWM = 9;        // Pin con salida PWM
int boton = 3;         // Botón en INT1
volatile bool subir = true; // Dirección: true = aumentar, false = disminuir
int brillo = 0;        // Nivel actual de brillo (0-255)

void setup() {
  pinMode(ledPWM, OUTPUT);
  pinMode(boton, INPUT_PULLUP);

  // Configurar interrupción externa en pin 3 (INT1)
  attachInterrupt(digitalPinToInterrupt(boton), cambiarDireccion, FALLING);
}

void loop() {
  // Escribir el valor actual en el pin PWM
  analogWrite(ledPWM, brillo);

  // Cambiar el brillo poco a poco
  if (subir) {
    brillo++;
    if (brillo >= 255) brillo = 255; // Límite superior
  } else {
    brillo--;
    if (brillo <= 0) brillo = 0;     // Límite inferior
  }

  delay(10); // Controla la velocidad del efecto
}

// ===== Rutina de interrupción =====
void cambiarDireccion() {
  subir = !subir; // Invierte la dirección
}