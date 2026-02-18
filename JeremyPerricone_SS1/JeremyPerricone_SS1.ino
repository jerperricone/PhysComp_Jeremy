//declare variables 
int ledPins[] = {4,5,6,7}; //Pin #'s
int count = 0; //Pattern Count
int timestop = 200; //Base delay time 




void setup() {
  //Initialize output pins
for (int i = 0; i < 4; i++) {
  pinMode(ledPins[i], OUTPUT);
  }
}
void loop() {
  // LED Patterns

//Adds 1 to pattern counter
count++; 

//Pattern 1 - Forward Chase 
if (count < 3)
{
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledPins[i], HIGH);
    delay(timestop);
    digitalWrite(ledPins[i], LOW);
  }
}

//Pattern 2 - Backwards Chase
else if (count < 5)
{
  for (int i = 4 - 1; i >= 0; i--) {
    digitalWrite(ledPins[i], HIGH);
    delay(timestop);
    digitalWrite(ledPins[i], LOW);
  }
}

//Pattern 3 - Two by Two 
else if (count < 7)
{
  digitalWrite(ledPins[0], HIGH);
  digitalWrite(ledPins[2], HIGH);
  digitalWrite(ledPins[1], LOW);
  digitalWrite(ledPins[3], LOW);
  delay(timestop);
  digitalWrite(ledPins[0], LOW);
  digitalWrite(ledPins[2], LOW);
  digitalWrite(ledPins[1], HIGH);
  digitalWrite(ledPins[3], HIGH);
  delay(timestop);
}

//Pattern 4 - Inside/Outside 
else if (count < 9)
{
  digitalWrite(ledPins[1], HIGH);
  digitalWrite(ledPins[2], HIGH);
  digitalWrite(ledPins[0], LOW);
  digitalWrite(ledPins[3], LOW);
  delay(timestop);
  digitalWrite(ledPins[1], LOW);
  digitalWrite(ledPins[2], LOW);
  digitalWrite(ledPins[0], HIGH);
  digitalWrite(ledPins[3], HIGH);
  delay(timestop);
}

//Pattern 5 - Bounce On & Hold
else if (count < 10)
{
  //Turn off LEDs from previous pattern 
  digitalWrite(ledPins[0], LOW);
  digitalWrite(ledPins[3], LOW);
  delay(timestop);

  //Bounce between pins 3-0
  for (int i = 3; i >= 0; i--) {
    digitalWrite(ledPins[i], HIGH);
    delay(timestop);
    digitalWrite(ledPins[i], LOW);
  }
  digitalWrite(ledPins[0], HIGH);
  
  //Bounce between pins 3-1
  for (int i = 3; i >= 1; i--) {
    digitalWrite(ledPins[i], HIGH);
    delay(timestop);
    digitalWrite(ledPins[i], LOW);
  }
  digitalWrite(ledPins[1], HIGH);

  //Bounce between pins 3-2
  for (int i = 3; i >= 2; i--) {
    digitalWrite(ledPins[i], HIGH);
    delay(timestop);
    digitalWrite(ledPins[i], LOW);
  }
  digitalWrite(ledPins[2], HIGH);
  delay(timestop);

//Turn pin 3 on 
digitalWrite(ledPins[3], HIGH);
delay(timestop);
}

//Pattern 6 - Bounce Off & Hold 
else if (count < 11)
{
  //Bounce between pins 3-0
  for (int i = 3; i >= 0; i--) {
    digitalWrite(ledPins[i], LOW);
    delay(timestop);
    digitalWrite(ledPins[i], HIGH);
  }
  digitalWrite(ledPins[0], LOW);
  
  //Bounce between pins 3-1
  for (int i = 3; i >= 1; i--) {
    digitalWrite(ledPins[i], LOW);
    delay(timestop);
    digitalWrite(ledPins[i], HIGH);
  }
  digitalWrite(ledPins[1], LOW);

  //Bounce between pins 3-2
  for (int i = 3; i >= 2; i--) {
    digitalWrite(ledPins[i], LOW);
    delay(timestop);
    digitalWrite(ledPins[i], HIGH);
  }
  digitalWrite(ledPins[2], LOW);
  delay(timestop);

//Turn pin 3 off
digitalWrite(ledPins[3], LOW);
delay(timestop);
}

//Reset pattern counter
else if (count >= 11)
  {
  count = 0;
  }
}
