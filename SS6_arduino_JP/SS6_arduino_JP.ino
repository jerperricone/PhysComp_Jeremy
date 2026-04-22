//A AND B TRAFFIC CONTROL 

const int adcPin = 1; //pot pin
const int ledPin = 4; //led pin

int adcRead = 0; //pot value
int inByte = 0; //incoming serial data 

bool ledState = 0;  //led ON/OFF state

int currentTime = 0;  //variable to store current millis
int lastTime = 0;     //variable to store millis at moment of last event
int timerInterval = 1000; //amount of milliseconds for comparison

void setup() 
{
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, 0);
  analogReadResolution(10); //0-1023
  Serial.begin(9600);
}

void loop() 
{
  if (Serial.available()) //if serial buffer is greater than 0
  {
    inByte = Serial.read(); //storing incoming byte

    if (inByte == 'A') //if the incoming byte is 'A'
    {
      //read sensors
      digitalWrite(ledPin, 0);
      adcRead = analogRead(adcPin);

      //send sensor data
      Serial.print(adcRead); //pot value
      Serial.print('\n');

    }
    else if (inByte == 'B') 
    {
      Serial.println("Frequency Matched");

      currentTime = millis(); //store current time
  
      if (currentTime - lastTime >= timerInterval)  //if time passed is greater than interval
      {
        lastTime = currentTime; //store current time as last time 
        ledState = !ledState; //flip LED state
      }

  digitalWrite(ledPin, ledState); //set pin to current state 

    }
    else 
    {
      digitalWrite(ledPin, 0);
      Serial.println("Rx Byte is neither 'A' nor 'B'");
    }
  }
}
