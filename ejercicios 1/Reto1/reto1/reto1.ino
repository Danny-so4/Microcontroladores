const int buttonPin = 10;
const int ledPin = 2;
int ledState = LOW;
int buttonState = HIGH;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
}

void loop() { 
  if (digitalRead(buttonPin) == LOW) {
    ledState = !ledState;             
    digitalWrite(ledPin, ledState);   
	}
}