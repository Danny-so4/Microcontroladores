
const int led1 = 2;
const int led2 = 3;
const int botonOn = 10;
const int botonOff = 11;
bool emergencia = false;

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(botonOn, INPUT_PULLUP);
  pinMode(botonOff, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(botonOn) == LOW) emergencia = true;
  if (digitalRead(botonOff) == LOW) emergencia = false;

  if (emergencia) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
    delay(200);
    digitalWrite(led1, LOW);
    digitalWrite(led2, HIGH);
    delay(200);
  } else {
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
  }
}