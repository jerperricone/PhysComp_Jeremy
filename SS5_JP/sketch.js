//declare variables 

let serial; // declare variable for an instance of the serialport library
let portName = 'COM6';  // fill in your serial port name here
let options = { baudRate: 9600}; // change the baud rate to match your Arduino code
let outByte = 0; //8 bit data to send to microController 

buttonState = false; //on and off function
let brightness = 0; //brightness tracker 

//dimmer control 
let sliderY;
let sliderMin = 100;
let sliderMax = 425;
let dragging = false;

function setup() 
{
createCanvas(600, 600);
  textAlign(CENTER, CENTER);
  textSize(36);

  sliderY = sliderMax; //start at 0 brightness

  //P5 SerialPort Setup
  serial = new p5.SerialPort();             // make a new instance of the serialport library
  serial.on('list', printList);             // set a callback function for the serialport list event
  serial.on('connected', serverConnected);  // set callback for connecting to the server
  serial.on('open', portOpen);              // set callback for the port opening
  serial.on('data', serialEvent);           // set callback for when new data received
  serial.on('error', serialError);          // set callback for errors
  serial.on('close', portClose);            // set callback for closing the port
  serial.list();                            // list the serial ports
  serial.open(portName, options);           // open a serial port
}


function draw() 
{
  background(220);

  //ON BUTTON 
if (buttonState) 
    {
      fill(0, 255, 0); //fill green when on
    } 
  else 
    {
      fill(255); //fill white when off 
    }
  stroke(0); //black outline 
  rect(150, 100, 225, 200); //on button dimensions 

  //"ON" TEXT 
  fill(0);
  noStroke();
  text('ON', 263, 200);

  //OFF BUTTON
if (!buttonState) 
    {
      fill(255, 0, 0); //fill red when off
    } 
  else 
    {
      fill(255); //fill white when on 
    }
  stroke(0); //black outline 
  rect(150, 300, 225, 200); //off button dimensions

  //"OFF" TEXT
  fill(0);
  noStroke();
  text('OFF', 263, 400);

  //DIMMER CONSTRAINT RECTANGLE 
  fill(50); //gray fill
  stroke(0); //black outline 
  rect(375, 100, 75, 400); //dimmer dimension 

  //SLIDER RECTANGLE 
  fill(255); //white fill 
  stroke(0); //black outline 
  rect(375, sliderY, 75, 75); //slider dimensions 

  // Update brightness from slider position
  brightness = map(sliderY, sliderMax, sliderMin, 0, 255);
  brightness = constrain(brightness, 0, 255);
  brightness = floor(brightness);

  //DETERMINE VALUES SENT TO ARDUINO 
  if(buttonState)
    {
      outByte = brightness;
    }
  else 
    {
      outByte = 0;
    }

  serial.write(outByte); //send byte across serial port
  

  fill(0);
  noStroke();
  text(brightness, 50, 50);

}

//MOUSE PRESSED FUNCTIONALITY 
function mousePressed() {
  // ON button click
  if (mouseX > 150 && mouseX < 375 && mouseY > 100 && mouseY < 300) 
    {
     buttonState = true;
    }

  // OFF button click
  if (mouseX > 150 && mouseX < 375 && mouseY > 300 && mouseY < 500) 
    {
      buttonState = false;
    }


  // Check if clicking dimmer rectangle
  if (mouseX > 375 && mouseX < 450) 
    {
      dragging = true;
    }
    else {
      dragging = false;
    }
}


//MOUSE DRAGGING FUNCTIONALITY 
function mouseDragged() 
{
  if (dragging) {
    sliderY = constrain(mouseY, sliderMin, sliderMax);
  }
}


function portOpen() //gets called when the serial port opens
{
  print("SERIAL PORT OPEN");
}

function portClose() //gets called when the serial port closes
{
  print("SERIAL PORT CLOSED");
}

function printList(portList) // gets called when the serial.list() function is called
{
  print("List of Available Serial Ports: ");
  for (var i = 0; i < portList.length; i++) 
  {
    print(i + portList[i]); //print list of available serial ports to console
  }
}

function serialEvent() // gets called when new serial data arrives
{
  //only sending data to microcontroller in this sketch, so not being used
}

function serialError(err) //gets called when there's an error
{
  print('ERROR: ' + err);
}

function serverConnected() //gets called when we connect to the serial server
{
  print("CONNECTED TO SERVER");
}