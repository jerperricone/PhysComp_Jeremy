//declare variables 
int ledPins[] = {4,5,6,7}; //LED Pin #'s
const int buzzer = 1; //Buzzer Pin # 
const int buttonPin = 42; //Button Pin #
const int potPin = 2; //Potentiometer Pin #
int potVal = 0; //Potentiometer Input Value 
int timer = 1000; //Buzzer Timer

void setup() {
  //Initialize output pins
for (int i = 0; i < 4; i++) {
  pinMode(ledPins[i], OUTPUT);
  }
  pinMode(buzzer, OUTPUT);

  //Initialize input pins
  pinMode(buttonPin, INPUT_PULLUP);
}



void loop() {
  potVal = analogRead(potPin);

  //Turn on first LED + buzzer 
  if (potVal >= 255) {
    digitalWrite(ledPins[0], HIGH);
  }

  //Turn on second LED + buzzer 
  if (potVal >= 511) {
    digitalWrite(ledPins[1], HIGH);
  }

  //Turn on third LED + buzzer 
  if (potVal >= 767) {
    digitalWrite(ledPins[2], HIGH);
  }

  //Turn on fourth LED + buzzer 
  if (potVal >= 1023) {
    digitalWrite(ledPins[3], HIGH);
  }

   // Reset with button press
  if (digitalRead(buttonPin) == LOW) {
    delay(50); // debounce
    if (digitalRead(buttonPin) == LOW) {
      for (int i = 0; i < 4; i++) {
        digitalWrite(ledPins[i], LOW);
      }
    }
  }


}
