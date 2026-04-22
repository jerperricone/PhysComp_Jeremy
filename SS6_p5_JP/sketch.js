let serial; // variable for instance of the serialport library
let portName = 'COM6'; // fill in your serial port name
let options = { baudRate: 9600}; // change the baud rate to match your Arduino code

let rxFlag = false; // flag to indicate when new data has been received
let firstContact = false; // flag to indicate when the first contact has been made with the serial port
let sensors = [0]; // declare array to hold incoming sensor data, and initialize with zeros
let pot = 0; // variable to hold potentiometer value

let number = 0; 
let numberMatch = false;

function setup() 
{
  //P5 Sketch Setup
  createCanvas(800, 600);
  textAlign(CENTER, CENTER);
  textSize(24);
  strokeWeight(4);
  stroke(127);

  number = floor(random(0, 150));

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
  background(255);

  //scale line
  stroke(0, 255, 0);
  line(100, 400, 700, 400);

  //indicator line
  stroke(0, 200, 0);
  let lineX = pot; //line x cordinate = pot value
  map(lineX, 100, 700, 0, 150); //map horozontal line distance from 0-150
  line(lineX, 375, lineX, 525); //x coordinate = pot

  textSize(50);
  noStroke();
  text(pot, 400, 300);

  if(pot == number) //make button appear when pot value = random number 
  {
    fill(255, 255, 0);
    rect(600, 700, 100, 200);

    fill(0);
    textSize(50);
    text('!', 650, 150);

    numberMatch = true;
  }

}

function mousePressed() //if mouse is pressed...
{
  if (numberMatch = true && mouseX > 600 && mouseX < 700 && mouseY > 100 && mouseY < 200) // if mouse postion is within button
  {
    rxFlag = true //make rxFlag true 

    if (rxFlag = true) //if rxFlag is true, send data
    {
      serial.write('B'); // send 'B' to the serial port to tell arduino to blink LED 
      number = floor(random(0, 150)); //select another random number 
      rxFlag = false //toggle rxFlag back to false
    }
    else  //if rxFlag is false, we want to recieve pot data
    {
      serial.write('A'); // send 'A' to the serial port to indicate that we want to recieve data from pot
    }
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
  
  if(rxFlag = false)  //if rxFlag is false, we want to receive data
  {
    let inString = serial.readStringUntil('\n'); // read the incoming string until you get a newline character
    if (inString.length > 0) 
    {
      print("Rx String: " + inString); // print the incoming string to the console
      sensors = split(inString, ','); // split the string into an array of sensor values
    
      if(sensors.length >= 1) // check if we see the pot
      {
        print(sensors); // print the array of sensor values to the console

        pot = Number(sensors[0]); // convert the sensor value to an integer
        pot = map(pot, 0, 1023, 0, 150); // map the potentiometer value from 0-1023 to 0-255
        pot = floor(pot); // round the potentiometer value to an integer

        print(" Pot: " + pot);  //print mapped sensor values to the console

        //now that we're done processing the incoming data, we can "call out" to our microcontroller, which respond with latest sensor data.
        serial.write('A');  // send 'A' to the serial port to indicate that we want the latest sensor data
      }
    }
  }
  else
  {
    let inString = serial.readStringUntil('\n'); // read the incoming string until you get a newline character
    print(inString); // print the incoming string to the console
  }
}

function serialError(err) //gets called when there's an error
{
  print('SERIAL ERROR: ' + err);
}

function serverConnected() //gets called when we connect to the serial server
{
  print("CONNECTED TO SERIAL SERVER");
}