/*
  CRISCO RADIO 

  MTEC2280 FINAL PROJECT BY JEREMY PERRICONE 

  Credits: 
  TFT graphics modified from tft_eSPI library example "TFT_Meters", by Bodner
*/


#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#include <FastLED.h>

TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

#define LOOP_PERIOD 35 // Display updates every 35 ms

float ltx = 0; // Saved x coord of bottom of needle
uint16_t osx = 160, osy = 180; // Saved x & y coords
uint32_t updateTime = 0; // time for next update

int old_analog =  -999; // Value last displayed

int value = 0; //0-100 value of needle placement 

//pot pins
const int potPin_1 = 16; //needle pot
const int potPin_2 = 5; //brightness pot
const int emptyPotPin = 18;


//button pins
const int buttonRed = 1;
const int buttonGreen = 2;
const int buttonReset = 42;

//led pins
const int ledPin_1 = 41;
const int backlightPin = 17;

// RGB LED strip
#define RGB_PIN 40 //strip data pin
#define NUM_LEDS 16

CRGB leds[NUM_LEDS];


//color variables 
int colorMode = 0;

uint16_t mainColor = TFT_RED;
uint16_t needleColor1 = TFT_RED;
uint16_t needleColor2 = TFT_MAROON;
uint16_t warningColor = TFT_RED;
uint16_t bgColor = TFT_BLACK;

CRGB stripColor = CRGB::Red;

//random number functionality 
int frequency = 0;
bool frequencyMatch = false;

bool warningVisible = false;

bool uiNeedsRedraw = false;



void setup(void) {
  tft.init();
  tft.setRotation(3); //horozontal orientation
  Serial.begin(9600);
  tft.fillScreen(bgColor);

  analogMeter(); // Draw analogue meter

  analogReadResolution(10); //0-1023

  //button setup
  pinMode(buttonRed, INPUT_PULLUP);
  pinMode(buttonGreen, INPUT_PULLUP);
  pinMode(buttonReset, INPUT_PULLUP);

  pinMode(ledPin_1, OUTPUT);
  digitalWrite(ledPin_1, HIGH);

  pinMode(backlightPin, OUTPUT);

  FastLED.addLeds<NEOPIXEL, RGB_PIN>(leds, NUM_LEDS);

  FastLED.clear();
  FastLED.show();

  FastLED.setBrightness(120); //strip led brightness 

  //start random string of numbers in a new place every reboot
  randomSeed(analogRead(emptyPotPin));
  frequency = random (0,101);

  updateTime = millis(); // Next update time
}


void loop() 
{

  //Color mode toggle 
  if (digitalRead(buttonRed) == LOW)
  {
    colorMode = 1;
    delay(150);
  }

  if (digitalRead(buttonGreen) == LOW)
  {
    colorMode = 0;
    delay(150);
  }

  //red
  if (colorMode == 1)
  {
    mainColor = TFT_RED;
    stripColor = CRGB::Red;
    needleColor1 = TFT_RED;
    needleColor2 = TFT_MAROON;
    warningColor = TFT_RED;
  }
  //green
  else
  {
    mainColor = TFT_GREEN;
    stripColor = CRGB::Green;
    needleColor1 = TFT_GREEN;
    needleColor2 = TFT_DARKGREEN;
    warningColor = TFT_GREEN;
  }

  static int lastMode = -1;

  if (colorMode != lastMode) 
  {
  tft.fillScreen(bgColor);
  analogMeter();

  uiNeedsRedraw = true;
  warningVisible = false; //reset render state

  lastMode = colorMode;
  }

  if (uiNeedsRedraw)
{
  if (frequencyMatch)
  {
    drawWarningOverlay();
    warningVisible = true;
  }

  uiNeedsRedraw = false;
}

  if (updateTime <= millis()) {
    updateTime = millis() + LOOP_PERIOD;

    //Map value from pot to move needle
    value = map(analogRead(potPin_1), 0, 1023, 0, 100); 

    //Control backlight brightness with pot
    int brightnessRaw = analogRead(potPin_2);
    int brightness = map(brightnessRaw, 0, 1023, 0, 255);
    analogWrite(backlightPin, brightness);

    plotNeedle(value, 0);

    drawWarning(); //call draw warning 

//LED Blink timer/logic

int distance = abs(value - frequency);

static unsigned long lastBlink = 0;
static bool ledState = false;

// numbers match
if (distance <= 2)
{
  digitalWrite(ledPin_1, HIGH);
  setStrip(true);
}

// within 20
else if (distance <= 20)
{
  int blinkSpeed = map(distance, 20, 3, 500, 80);

  if (millis() - lastBlink >= blinkSpeed)
  {
    lastBlink = millis();

    ledState = !ledState;

    digitalWrite(ledPin_1, ledState ? LOW : HIGH);

    setStrip(!ledState);
  }
}

// farther than 20
else
{
  digitalWrite(ledPin_1, HIGH);
  setStrip(true);
}


  static bool lastResetState = HIGH;
  bool resetPressed = digitalRead(buttonReset);

// detect button press edge
if (lastResetState == HIGH && resetPressed == LOW)
{
  if (frequencyMatch)
  {
    frequency = random(0, 101); // new random number
    frequencyMatch = false; // clear match state

    //warning goes away
    warningVisible = false;
    tft.fillRect(0, 160, 90, 80, bgColor);
  }
}

lastResetState = resetPressed;

    if (frequencyMatch && !warningVisible)
{
  // draw triangle once
  drawWarningOverlay();
  warningVisible = true;
}

if (!frequencyMatch && warningVisible)
{
  // erase only triangle area
  tft.fillRect(0, 160, 90, 80, bgColor);
  warningVisible = false;
}

      //Display value in top right corner
  tft.setTextColor(mainColor, bgColor);
  char buf[8]; dtostrf(value + 50, 4, 0, buf);
  strcat(buf, " Hz");
  tft.drawRightString(buf, 315, 5, 2);

  }
}


//Draw meter
void analogMeter()
{

  tft.setTextColor(mainColor);  // Text color 

  // Draw ticks every 5 degrees from -50 to +50 degrees (100 deg. FSD swing)
  for (int i = -50; i < 51; i += 5) {
    // Long scale tick length
    int tl = 15;

    // Coordinates of tick to draw
    float sx = cos((i - 90) * 0.0174532925);
    float sy = sin((i - 90) * 0.0174532925);
    uint16_t x0 = sx * (150 + tl) + 160;
    uint16_t y0 = sy * (150 + tl) + 210;
    uint16_t x1 = sx * 150 + 160;
    uint16_t y1 = sy * 150 + 210;

    // Short scale tick length
    if (i % 25 != 0) tl = 8;

    // Recalculate coords incase tick lenght changed
    x0 = sx * (150 + tl) + 160;
    y0 = sy * (150 + tl) + 210;
    x1 = sx * 150 + 160;
    y1 = sy * 150 + 210;

    // Draw tick
    tft.drawLine(x0, y0, x1, y1, mainColor);

    // Check if labels should be drawn, with position tweaks
    if (i % 25 == 0) {
      // Calculate label positions
      x0 = sx * (150 + tl + 10) + 160;
      y0 = sy * (150 + tl + 10) + 210;
      switch (i / 25) {
        case -2: tft.drawCentreString("50", x0, y0 - 12, 2); break;
        case -1: tft.drawCentreString("75", x0, y0 - 9, 2); break;
        case 0: tft.drawCentreString("100", x0, y0 - 6, 2); break;
        case 1: tft.drawCentreString("125", x0, y0 - 9, 2); break;
        case 2: tft.drawCentreString("150", x0, y0 - 12, 2); break;
      }
    }

    // Now draw the arc of the scale
    sx = cos((i + 5 - 90) * 0.0174532925);
    sy = sin((i + 5 - 90) * 0.0174532925);
    x0 = sx * 150 + 160;
    y0 = sy * 150 + 210;
    // Draw scale arc, don't draw the last part
    if (i < 50) tft.drawLine(x0, y0, x1, y1, mainColor);
  }

  plotNeedle(0, 0); // Put meter needle at 0
}


//Needle position

void plotNeedle(int value, byte ms_delay)
{

  if (value < -10) value = -10; // Limit value to emulate needle end stops
  if (value > 110) value = 110;

  // Move the needle until new value reached
  while (!(value == old_analog)) {
    if (old_analog < value) old_analog++;
    else old_analog--;

    if (ms_delay == 0) old_analog = value; // Update immediately id delay is 0

    float sdeg = map(old_analog, -10, 110, -160, -20); // Map value to angle
    // Calculate tip of needle coords
    float sx = cos(sdeg * 0.0174532925);
    float sy = sin(sdeg * 0.0174532925);

    // Calculate x delta of needle start (does not start at pivot point)
    float tx = tan((sdeg + 90) * 0.0174532925);

    // Erase old needle image
    tft.drawLine(160 + 20 * ltx - 1, 180 - 20, osx - 1, osy, bgColor);
    tft.drawLine(160 + 20 * ltx, 180 - 20, osx, osy, bgColor);
    tft.drawLine(160 + 20 * ltx + 1, 180 - 20, osx + 1, osy, bgColor);

    // Store new needle end coords for next erase
    ltx = tx;
    osx = sx * 98 + 160;
    osy = sy * 98 + 180;

    // Draw the needle in the new postion, magenta makes needle a bit bolder
    // draws 3 lines to thicken needle
    tft.drawLine(160 + 20 * ltx - 1, 180 - 20, osx - 1, osy, needleColor1);
    tft.drawLine(160 + 20 * ltx, 180 - 20, osx, osy, needleColor2);
    tft.drawLine(160 + 20 * ltx + 1, 180 - 20, osx + 1, osy, needleColor1);

    // Slow needle down slightly as it approaches new postion
    if (abs(old_analog - value) < 10) ms_delay += ms_delay / 5;

    // Wait before next update
    delay(ms_delay);
  }
}

void drawWarning()
{
  frequencyMatch = (abs(value - frequency) <= 2);

  Serial.write(frequencyMatch); //send 0 (false) or 1 (true) to p5
}

//draw warning triangle
void drawWarningOverlay()
{
  int x = 35;
  int y = 200;
  int s = 35;

  tft.fillTriangle(
    x, y - s,
    x - s, y + s,
    x + s, y + s,
    mainColor
  );

tft.drawTriangle(x, y - s, x - s, y + s, x + s, y + s, TFT_DARKGREY);
tft.drawTriangle(x+1, y - s, x - s + 1, y + s, x + s + 1, y + s, TFT_DARKGREY);
tft.drawTriangle(x-1, y - s, x - s - 1, y + s, x + s - 1, y + s, TFT_DARKGREY);

  tft.setTextColor(TFT_BLACK, mainColor);
  tft.setTextDatum(MC_DATUM);
  tft.drawCentreString("!", x, y, 4);
}

//set up strip led logic 
void setStrip(bool on)
{
  if (on)
  {
    fill_solid(leds, NUM_LEDS, stripColor);
  }
  else
  {
    fill_solid(leds, NUM_LEDS, CRGB::Black);
  }

  FastLED.show();
}