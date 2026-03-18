//declare variables

let serial; // declare variable for an instance of the serialport library
let portName = 'COM6';  //serial port name 
let options = { baudRate: 9600}; // baud rate to match Arduino 

let inData; // declare variable for storing incoming serial data
 
function setup() //setup function runs once at beginning
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

  createCanvas(800, 600); //canvas size
}

function draw() 
{
  background(0, 150, 255);  //background color

  fill(255 - inData, inData, 0); // fill changes from red to green based on pot val
  let position = map(inData, 0, 255, 50, width - 50); // scale incoming data from 0 to 255 into circle horozontal position
  circle(position, height/2, 50);  // draw circle in center of canvas with width based on serial data

  fill(255,0,0); //red circle fill
  circle(50,50,50); //red circle placement

  fill(0,255,0); //green circle fill
  circle(750,50,50); //green circle placement
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
  inData = Number(serial.read()); //Store incoming data as a number
  //print(inData);
}
 
function serialError(err) //gets called when there's an error
{
  print('ERROR: ' + err);
}
 
function portClose() //gets called when the serial port closes
{
  print("*____SERIAL PORT CLOSED");
}