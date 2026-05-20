//p5 final project indicator 

let serial; // declare variable for an instance of the serialport library
let portName = 'COM6';  // fill in your serial port name here
let options = { baudRate: 9600}; // change the baud rate to match your Arduino code

let inData = 0; // declare variable for storing incoming serial data

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

  //TYPICAL P5.JS SETUP
  createCanvas(800, 800); //set size of canvas
  textSize(72); // set text size
  textAlign(CENTER, CENTER);  // set text alignment
}

function draw() 
{

  background(0);

  if (inData === 1)
  {
    // Triangle dimensions 
    let cx = width / 2;
    let cy = height / 2;
    let size = 300;

    //Red triangle
    fill(255, 0, 0);
    stroke(80);
    strokeWeight(6);

    triangle(
    cx, cy - size / 2, // top
    cx - size / 2, cy + size / 2, // bottom left
    cx + size / 2, cy + size / 2  // bottom right
    );

    //Exclaimation point
    fill(0);
    noStroke();
    textSize(180);
    text("!", cx, cy + 30);
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