
/*
 * This is the code for ARCA (Adorable Remote Controlled Android) that implements two 8 by 8 LED's for eyes
 * as well as two gear motors to move around on wheels.  The robot is controlled by a remote control that
 * allows the user to change the robot's emotions and move the robot.  
 * By Rachel Klesius
 */

#include <LedControl.h>  //for controlling the 8 by 8 LED MCU
#include <IRremote.h> //for the IR remote

//IR sensor pin
#define RECVR 11

//define pins for LED displays
#define leftDIN 10
#define leftCS 9
#define leftCLK 8

#define rightDIN 7
#define rightCS 6
#define rightCLK 5

//define pins for gear motors
#define LMOTOR 3
#define RMOTOR 2

//define emotions
byte neutral[8] = {0x00,0x00,0x30,0x78,0x78,0x30,0x00,0x00};
byte happy[8] = {0x70,0x08,0x04,0x04,0x04,0x04,0x08,0x70};
byte sad[8] = {0x08,0x10,0x20,0x20,0x20,0x20,0x10,0x08};
byte leftAngry[8] = {0x00,0x00,0x60,0xF1,0xF2,0x64,0x08,0x10};
byte rightAngry[8] = {0x10,0x08,0x64,0xF2,0xF1,0x60,0x00,0x00};
byte lookRight[8] = {0x18,0x3C,0x3C,0x18,0x00,0x00,0x00,0x00};
byte lookLeft[8] = {0x00,0x00,0x00,0x00,0x18,0x3C,0x3C,0x18};
byte leftLove[8] = {0x0C,0x1E,0x3F,0x7E,0xFC,0x7E,0x3F,0x1E};
byte rightLove[8] = {0x1E,0x3F,0x7E,0xFC,0x7E,0x3F,0x1E,0x0C};
byte lookUp[8] = {0x00,0x00,0x06,0x0F,0x0F,0x06,0x00,0x00};
byte closed[8] = {0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10};

//create ledControl object
LedControl leftEye = LedControl(leftDIN, leftCLK, leftCS, 0);
LedControl rightEye = LedControl(rightDIN, rightCLK, rightCS, 0);

//bool to blink or not, won't blink with happy
bool canBlink = true;
int lastBlink = 0;

//variables to print emotions
byte *leftEmotion = neutral;
byte *rightEmotion = neutral;


//create IR sensor objects
IRrecv irrecv(RECVR);

  
void setup() {
  Serial.begin(9600);
  
  leftEye.shutdown(0, false);  //get out of power saving mode
  rightEye.shutdown(0, false);
  leftEye.setIntensity(0, 5); //maximum brightness
  rightEye.setIntensity(0, 5);
  leftEye.clearDisplay(0);  //clear the display
  rightEye.clearDisplay(0);

  //initialize the pins of the wheels
  pinMode(LMOTOR, OUTPUT);
  pinMode(RMOTOR, OUTPUT);

  irrecv.enableIRIn();  //starts the reciever
}

void loop() {
  decode_results results;

  lastBlink++;
  printByte(leftEmotion, rightEmotion);
  
  if (irrecv.decode(&results))  //if results are recieved
  {
    if (results.value == 0xFFA25D)  //button 1 pressed, neutral
    {
      canBlink = true;
      leftEmotion = neutral;
      rightEmotion = neutral;
    }
    if (results.value == 0xFF629D)  //button 2 pressed, happy
    {  
      canBlink = false;
      leftEmotion = happy;
      rightEmotion = happy;
    }
    if (results.value == 0xFFE21D)  //button 3 pressed, sad
    {  
      canBlink = false;
      leftEmotion = sad;
      rightEmotion = sad;
    }
    if (results.value == 0xFF22DD)  //button 4 pressed, love
    {
      canBlink = true;
      leftEmotion = leftLove;
      rightEmotion = rightLove;
    }
    if (results.value == 0xFF02FD)  //button 5 pressed, angry
    {
      canBlink = true;
      leftEmotion = leftAngry;
      rightEmotion = rightAngry;
    }
    if (results.value == 0xFFC23D)  //button 6 pressed, silly
    {
      canBlink = true;
      leftEmotion = lookRight;
      rightEmotion = lookLeft;
    }
    if (results.value == 0xFFE01F)  //button 7 pressed, look left
    {
      canBlink = true;
      leftEmotion = lookLeft;
      rightEmotion = lookLeft;
    }
    if (results.value == 0xFFA857)  //button 8 pressed, look up
    {
      canBlink = true;
      leftEmotion = lookUp;
      rightEmotion = lookUp;
    }
    if (results.value == 0xFF906F)  //button 9 pressed, look right
    {
      canBlink = true;
      leftEmotion = lookRight;
      rightEmotion = lookRight;
    }

    //for the motor control
    if (results.value == 0xFF18E7)  //up button pressed
    {
      digitalWrite(LMOTOR, HIGH);
      digitalWrite(RMOTOR, HIGH);
    }
    if (results.value == 0xFF10EF)  //Left button pressed
    {
      digitalWrite(LMOTOR, LOW);
      digitalWrite(RMOTOR, HIGH);
    }
    if (results.value == 0xFF5AA5)  //Right button pressed
    {
      digitalWrite(RMOTOR, LOW);
      digitalWrite(LMOTOR, HIGH);
    }
    if (results.value == 0xFF38C7)  //OK button pressed (brake)
    {
      digitalWrite(RMOTOR, LOW);
      digitalWrite(LMOTOR, LOW);  
    }
    irrecv.resume();  //prepare for next value
  }
  if(canBlink && lastBlink == 150)
  {
    printByte(closed, closed);
    delay(50);
    lastBlink = 0;
  }
}

//prints the data to the 8 by 8 display
void printByte(byte left[], byte right[])
{
  for(int i=0; i < 8; i++)
  {
    leftEye.setRow(0,i,left[i]);
    rightEye.setRow(0,i,right[i]);
  }
  return;
}

