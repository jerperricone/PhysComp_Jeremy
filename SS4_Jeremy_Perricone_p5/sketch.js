//declare variables 

let serial; // declare variable for an instance of the serialport library
let portName = 'COM6';  // fill in your serial port name here
let options = { baudRate: 9600}; // change the baud rate to match your Arduino code

let inData = 0; // variable for storing incoming serial data
let sensors = [0, 0, 0, 0]; // array to hold sensor values: [button1, button2, pot1, pot2]

let r, g, b; // variables for circle color
let diameter = 150; // diameter of circle

function setup() 
{
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

 //TYPICAL P5.JS SETUP
  createCanvas(1000, 800); //set size of canvas
}

function draw() {
  background(255);

  r = map(sensors[1], 0, 1023, 0, 255); //map pot 1 to red value
  g = map(sensors[2], 0, 1023, 0, 255); //map pot 2 to green value
  b = map(sensors[3], 0, 1023, 0, 255); //map pot 3 to blue value

{
  if (sensors[0] === 1) //if button is pressed
  {
    fill(r, g, b); //first circle - only red 
  }
  else 
  {
    fill(r, 0, 0); //first circle - rgb
  }

  circle(200, 400, diameter); //circle 1 
}

{
  if (sensors[0] === 1) //if button is pressed
  {
    fill(r, g, b); //second circle - only green
  }
  else 
  {
    fill(0, g, 0); //second circle - rgb
  }

  circle(500, 400, diameter); //circle 2
}

{
  if (sensors[0] === 1) //if button is pressed
  {
    fill(r, g, b); //third circle - only red 
  }
  else 
  {
    fill(0, 0, b); //third circle - rgb
  }

  circle(800, 400, diameter); //circle 3
}
}



function printList(portList) // gets called when the serial.list() function is called
{
  print("List of Available Serial Ports: ");
  for (var i = 0; i < portList.length; i++) 
  {
    print(i + portList[i]); //print list of available serial ports to console
  }
}

function serverConnected() //gets called when we connect to the serial server
{
  print("CONNECTED TO SERVER");
}
 
function portOpen() //gets called when the serial port opens
{
  print("SERIAL PORT OPEN");
}
 
function serialEvent() //gets called when new data arrives
{
  let inString = serial.readStringUntil('\n'); // read until newline character
  if (inString.length > 0) //if there's data in the string
  {
    sensors = split(inString, ','); // split the string at commas and store in array
    
    for (let i = 0; i < sensors.length; i++) 
    {
      // Number() function converts string to number
      sensors[i] = Number(sensors[i]); // convert every element in array to numbers

      // You can also use parseInt() function, which takes a second argument for the base (radix).
      // A base of 10 is for decimal numbers, base of 16 is for hexadecimal, base of 2 is for binary.
      // sensors[i] = parseInt(sensors[i], 10); // converts every element in array to decimal number
    }
    print(sensors);
    //print(sensors);
  } 
}
 
function serialError(err) //gets called when there's an error
{
  print('ERROR: ' + err);
}
 
function portClose() //gets called when the serial port closes
{
  print("*____SERIAL PORT CLOSED");
}