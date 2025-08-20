const int buttonPin = 10;
const int ledPin1 = 4;
const int ledPin2 = 2;
const int ledPin3 = 3;
const int ledPin4 = 5;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin1, OUTPUT);
  digitalWrite(ledPin1, LOW);
  pinMode(ledPin2, OUTPUT);
  digitalWrite(ledPin2, HIGH);
  pinMode(ledPin3, OUTPUT);
  digitalWrite(ledPin3, LOW);
  pinMode(ledPin4, OUTPUT);
  digitalWrite(ledPin4, LOW);
}

void loop() { 
  if (digitalRead(buttonPin) == LOW) {
      delay(50);
      digitalWrite(ledPin2, LOW);
	  digitalWrite(ledPin3, HIGH);
      delay(700);
      digitalWrite(ledPin3, LOW);
      digitalWrite(ledPin4, HIGH);
      digitalWrite(ledPin1, HIGH);
      delay(1500);
      digitalWrite(ledPin4, LOW);
      digitalWrite(ledPin1, LOW);
      digitalWrite(ledPin2, HIGH);
	}
}