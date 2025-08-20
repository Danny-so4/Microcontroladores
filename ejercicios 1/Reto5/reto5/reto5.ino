const int led = 2; 
const int botonMas = 10;
const int botonMenos = 11;
int brillo = 128; 

void setup() {
  pinMode(led, OUTPUT);
  pinMode(botonMas, INPUT_PULLUP);
  pinMode(botonMenos, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(botonMas) == LOW && brillo < 255) {
    brillo += 25;
    delay(200);
  }
  if (digitalRead(botonMenos) == LOW && brillo > 0) {
    brillo -= 25;
    delay(200);
  }
  analogWrite(led, brillo);
}
