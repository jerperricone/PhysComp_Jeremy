//declare variables

const int adcPin = 2; //pot pin
int adcRead = 0;  //pot value

//software timer variables
int lastTime = 0;
int currentTime = 0;
int timerInterval = 2;

void setup() 
{
  Serial.begin(9600);
  analogReadResolution(8);
}

void loop() 
{
  currentTime = millis(); 
  if (currentTime - lastTime >= timerInterval)  
  {
    lastTime = currentTime; //store current time 
    adcRead = analogRead(adcPin); //read value on

    Serial.write(adcRead);  //send binary
  }
}