int leds[4] = {2,3,4,5};
int boton = 10;
int numero = 0, ultimoEstado = HIGH;

void setup() {
  for(int i=0;i<4;i++) pinMode(leds[i], OUTPUT);
  pinMode(boton, INPUT_PULLUP);
}

void mostrarBinario(int n) {
  for(int i=0; i<4; i++)
    digitalWrite(leds[i], (n >> i) & 1);
}

void loop() {
  int lectura = digitalRead(boton);
  if (lectura == LOW && ultimoEstado == HIGH) {
    numero = (numero + 1) % 16;
    mostrarBinario(numero);
    delay(200);
  }
  ultimoEstado = lectura;
}
