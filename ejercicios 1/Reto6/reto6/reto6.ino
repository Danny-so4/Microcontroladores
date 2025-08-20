int leds[3] = {2,3,4};
int botones[3] = {9,10,11};
int secuencia[10];
int nivel = 0;

void setup() {
  for(int i=0;i<3;i++) {
    pinMode(leds[i], OUTPUT);
    pinMode(botones[i], INPUT_PULLUP);
  }
  randomSeed(analogRead(0));
}

void mostrarSecuencia() {
  for(int i=0; i<=nivel; i++) {
    digitalWrite(leds[secuencia[i]], HIGH);
    delay(400);
    digitalWrite(leds[secuencia[i]], LOW);
    delay(200);
  }
}

bool verificar() {
  for(int i=0;i<=nivel;i++) {
    while(digitalRead(botones[0])==HIGH &&
          digitalRead(botones[1])==HIGH &&
          digitalRead(botones[2])==HIGH);
    for(int j=0;j<3;j++) {
      if(digitalRead(botones[j])==LOW) {
        if(j != secuencia[i]) return false;
        digitalWrite(leds[j], HIGH); delay(200); digitalWrite(leds[j], LOW);
        while(digitalRead(botones[j])==LOW);
      }
    }
  }
  return true;
}

void loop() {
  secuencia[nivel] = random(0,3);
  mostrarSecuencia();
  if (verificar()) {
    nivel++;
    if(nivel>=10) nivel=0;
  } else {
    nivel=0; // Reinicia
    delay(1000);
  }
}