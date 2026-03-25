//DEFINE VARIABLES

const int potPin_1 = 4;
const int potPin_2 = 5;
const int potPin_3 = 6;
const int buttonPin = 7;

//button variables
bool buttonState = 0;

//pot variables
int potRead_1 = 0;
int potRead_2 = 0;
int potRead_3 = 0;

//software timing variables
int lastTime = 0;
int currentTime = 0;
int timerInterval = 5;  //in ms



void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  analogReadResolution(10);  //set ADC read to 0-1023 range
  Serial.begin(9600);
}

void loop() {
  //read adc
  potRead_1 = analogRead(potPin_1);
  potRead_2 = analogRead(potPin_2);
  potRead_3 = analogRead(potPin_3);

  buttonState = !digitalRead(buttonPin);

  currentTime = millis();
  if (currentTime - lastTime >= timerInterval) {
    lastTime = currentTime;
    Serial.print(buttonState);
    Serial.print(',');
    Serial.print(potRead_1);
    Serial.print(',');
    Serial.print(potRead_2);
    Serial.print(',');
    Serial.print(potRead_3);
    Serial.print('\n');
  }
}
