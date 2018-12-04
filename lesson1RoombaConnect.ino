#include<SoftwareSerial.h>
//hi

// pin assignments for arduino to roomba 7 mini-din port

int rxPin = 10;        // from arduino(10) to roomba pin(4) white wire
int txPin = 11;       // to arduino(11) from roomba pin(3) red wire
int ddPin = 5;        // device detect used to wake up roomba

int inByte = 0;       // sensro data read from serial buffer
byte wallData[4];     // light bump sensor data bytes read from buffer
int i = 0;            // byte counter for reading sensor data

SoftwareSerial mySerial(rxPin, txPin);  
// new port with 10rx and 11tx

void setup() {
  Serial.begin(9600);    
  // start the serial port for the serial monitor
  Serial.println("hello dave");      

  pinMode (ddPin, OUTPUT);            
  // set ddPin(5) to output, that's from arduino to roomba
  
  mySerial.begin(19200);       
  // start software serial for communicating with roomba
  // use software serial because roomba's tx doesn't always
  // carry enough voltage to meet arduino's requirements
  
  delay(10);             
  // set a short delay to allow the command to propegate
 
  // check comminication between Arduino and roomba 
  if (mySerial.available())   
  //First order of business: listen to Roomba
    Serial.println(mySerial.read());   
    //writes to USB input from soft serial if connected to lapt  

// wake up roomba with a series of pulses to the dd pin on the roomba  
wakeUp();
  //digitalWrite(ddPin, HIGH);
  //delay(100);
  //digitalWrite(ddPin, LOW);
  //delay(500);
  //digitalWrite(ddPin, HIGH);
  //delay(2000);
  Serial.println("i'm awake.");
  //wakeUp();

// put roomba into passive mode
  mySerial.write(128);  // put roomba into passive mode                        
  delay(10);            // wait for mode change to propegate
  
  mySerial.write(130);  // put roomba into safe mode
  Serial.println("safe mode");
                             
  delay(10);  // allow mode change to propegate

//DRIVES IN BOX (2 m * 1.5 m)
  driveWheels(200,200);
  delay(8000);
  driveWheels(400, -25);
  delay(1000);
  driveWheels(200,200);
  delay(6000);
  driveWheels(400, -25);
  delay(1000);
  driveWheels(200,200);
  delay(8000);
  driveWheels(400, -25);
  delay(1000);
  driveWheels(200,200);
  delay(6000);
  stopDrive();

//DRIVES IN CIRCLE
  driveWheels(150, 40);
  delay(20000);
  stopDrive(); 

//DRIVES IN EQUILATERAL TRAINGLE
  driveWheels(200,200);
  delay(4000);
  driveWheels(250, -300);
  delay(1000);
  driveWheels(200,200);
  delay(4000);
  driveWheels(250, -300);
  delay(1000);
  driveWheels(200,200);
  delay(4000);
  driveWheels(250, -300);
  delay(1000);
  stopDrive();

}


void loop() {

  driveWheels(200,200);
  delay(8000);
  driveWheels(400, -25);
  delay(1000);
  
  mySerial.write(142); // send a senosr packet the next line indicate the packet id
  mySerial.write(9);  // as an examle of reading sensor data read the left cliff sensor
  delay(250);
  if(mySerial.available() > 0){
    inByte = mySerial.read();
    Serial.println(inByte);
  }
 
}

 void goForward(void)
 {
  Serial.println("in goForward");
    mySerial.write(byte(137));
    mySerial.write(byte(0));
    mySerial.write(byte(100));
    mySerial.write(byte(0x80));
    //mySerial.write(byte (0));    // go forward

 } // end goForward

/***********************************************
 * direct drive wheel motors
 * 4 bytes are
 * right velocity high byte
 * right velocity low byte
 * left velocity high byte
 * left velocitn low byte
 * 
 * examples of both bitwise shift right and highByte, lowByte
 * to parse the argument into 2 bytes
 * 
 * constrain is used to ensure that the parameter values are within 
 * range for driving roomba's wheels
 /************************************************/
void driveWheels(int right, int left)
{
  constrain(right, -500, 500);
  constrain(left, -500, 500);

  Serial.println("in driveWheels");
  
  mySerial.write(145);
  mySerial.write(right >> 8);
  mySerial.write(right);
  mySerial.write(left >> 8);
  mySerial.write(left);

  mySerial.write(highByte(right));
  mySerial.write(lowByte(right));
  mySerial.write(highByte(left));
  mySerial.write(lowByte(left));
 } // end drive wheels
 
/*******************************************
 * stop roomba, send drive opcode (137) 
 * with all zero arguments
 ********************************************/
void stopDrive(void){
  Serial.println("Stopped drive");
  mySerial.write(byte(137));
  mySerial.write(byte(0));
  mySerial.write(byte(0));
  mySerial.write(byte(0));
  mySerial.write(byte(0));  
} // end stopdrive()

/*******************************************
 * wake up roomba by pulsing dd pin 5 high low high
 ********************************************/
 void wakeUp (void)
{
  //setWarningLED(ON);
  Serial.println("wake up, wake up.");
  digitalWrite(ddPin, HIGH);
  delay(100);
  digitalWrite(ddPin, LOW);
  delay(500);
  digitalWrite(ddPin, HIGH);
  delay(2000);
}
