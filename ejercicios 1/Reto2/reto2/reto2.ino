int led = 2;
const int boton = 10;
int botonstate = 0;

void setup() {
  for (int pin = 2; pin < 10; pin++) {
    pinMode(pin, OUTPUT);
  }
  pinMode(boton, INPUT_PULLUP);
}
void loop() {
  botonstate = digitalRead(boton);

  if (botonstate == LOW) {

      for (led = 2; led < 10; led++) {
        digitalWrite(led, HIGH);
        delay(100);
      }
    } 
    else {
      for (led = 9; led > 1; led--) {
        digitalWrite(led, LOW);
        delay(100);
      }
    
  }
}
