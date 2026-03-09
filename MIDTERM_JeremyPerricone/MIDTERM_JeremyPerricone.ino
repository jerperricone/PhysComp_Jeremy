//declare variables 
int ledPins[] = {4,5,6,7}; //LED Pin #'s
const int buzzerPin = 1; //Buzzer Pin # 
const int buttonPin = 42; //Button Pin #
const int potPin = 2; //Potentiometer Pin #

int potVal = 0; //Potentiometer Input Value 

int A = 110; //A note frequency 
int octave = 2; //octave multiplier

bool reset = false; //Flag for button reset 

bool buzzerFlag = false; //To tell program if the buzzer has beeped yet 
unsigned long buzzerStart = 0; 
int buzzerDuration = 500;

bool ledTriggered[4] = {false,false,false,false}; //To tell program if leds have turned on 



void setup() {

  //Initialize output pins
for (int i = 0; i < 4; i++) {
  pinMode(ledPins[i], OUTPUT);
  }
  pinMode(buzzerPin, OUTPUT);

  //Initialize input pins
  pinMode(buttonPin, INPUT_PULLUP);

  //Set pot resolution 
  analogReadResolution(10);
}


void loop() {

  //Read pot value 
  potVal = analogRead(potPin);

  //Unlock system after button press 
  if (reset && potVal < 10) {
    reset = false;
  }

  //Allows LEDs to turn on after button press 
  if (!reset) {
  

    //Turn on first LED + buzzer 
    if (potVal >= 255 && !ledTriggered[0]) {
        digitalWrite(ledPins[0], HIGH);
        ledTriggered[0] = true;

        buzzerDuration = 200;
        tone(buzzerPin, A * octave);
        buzzerFlag = true;
        buzzerStart = millis();
      }
    

    //Turn on second LED + buzzer 
    if (potVal >= 511 && !ledTriggered[1]) {
      digitalWrite(ledPins[1], HIGH);
      ledTriggered[1] = true;

      buzzerDuration = 200;
      tone(buzzerPin, A * octave);
      buzzerFlag = true;
      buzzerStart = millis();
    }
    

    //Turn on third LED + buzzer 
    if (potVal >= 767 && !ledTriggered[2]) {
      digitalWrite(ledPins[2], HIGH);
      ledTriggered[2] = true;

      buzzerDuration = 200;
      tone(buzzerPin, A * octave);
      buzzerFlag = true;
      buzzerStart = millis();
    }


    //Turn on fourth LED + buzzer 
    if (potVal >= 1023 && !ledTriggered[3]) {
      digitalWrite(ledPins[3], HIGH);
      ledTriggered[3] = true;
      buzzerDuration = 800;
      tone(buzzerPin, A * octave * octave);
      buzzerFlag = true;
      buzzerStart = millis();
    }

  }


  //Stop buzzer 
  if (buzzerFlag && millis() - buzzerStart >= buzzerDuration) {
    noTone(buzzerPin);
    buzzerFlag = false;
  }


   // Reset with button press
  if (digitalRead(buttonPin) == LOW) {
    delay(50); // debounce
    if (digitalRead(buttonPin) == LOW) {
      for (int i = 0; i < 4; i++) {
        digitalWrite(ledPins[i], LOW);
        ledTriggered[i] = false;
      }
      reset = true; //stops lights from turning on until pot reaches 0
    }
  }
}
