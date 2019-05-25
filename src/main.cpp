#include <Arduino.h>
/*
  C++ code for ATTiny85 driving a 74HC595 Shift Register
  Platformio with SparkFun Tiny Programmer
  
*/

//latchPin connected to 74HC595 ST_CP  pin 12
int latchPin = 2;

//clockPin connected to 74HC595 SH_CP pin 11
int clockPin = 3;

//dataPin connected to 74HC595 DS pin 14
int dataPin = 0;

//information containers for passing data to the do the shift
byte data;
byte outArray[8];

void shiftOut(int dataPin, int clockPin, byte myDataOut) {
  //Shifts 8 bits out MSB first
  //on the rising edge of the clock,
  //clock idles low

  //internal function setup
  int i=0;
  int pinState;
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  //shift register initialisation
  digitalWrite(dataPin, 0);
  digitalWrite(clockPin, 0);

  for (i=7; i>=0; i--)  {
    digitalWrite(clockPin, 0);

    if ( myDataOut & (1<<i) ) {
      pinState= 1;
    }
    else {  
      pinState= 0;
    }

    //Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(dataPin, pinState);
    //register shifts bits on upstroke of clock pin  
    digitalWrite(clockPin, 1);
    //zero the data pin after shift to prevent bleed through
    digitalWrite(dataPin, 0);
  }

  //stop shifting
  digitalWrite(clockPin, 0);
}


//blinks all n times
void blinkAll(int n, int d) {
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, 0);
  shiftOut(dataPin, clockPin, 0);
  digitalWrite(latchPin, 1);
  delay(200);
  for (int x = 0; x < n; x++) {
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, 255);
    shiftOut(dataPin, clockPin, 255);
    digitalWrite(latchPin, 1);
    delay(d);
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, 0);
    shiftOut(dataPin, clockPin, 0);
    digitalWrite(latchPin, 1);
    delay(d);
  }
}

void setup() {
  pinMode(latchPin, OUTPUT);
  outArray[0] = 1;   //00000001
  outArray[1] = 2;   //00000010
  outArray[2] = 4;   //00000100
  outArray[3] = 8;   //00001000
  outArray[4] = 16;  //00010000
  outArray[5] = 32;  //00100000
  outArray[6] = 64;  //01000000
  outArray[7] = 128; //10000000
  
  //blinkAll(2, 500); 
}

void loop() {

  for (int j = 0; j < 8; j++) {
    data = outArray[j];
    //ground latchPin and hold low while transmitting
    digitalWrite(latchPin, 0);
    //send date
    shiftOut(dataPin, clockPin, data);
    //return latch pin high to signal that data sent is done
    digitalWrite(latchPin, 1);
    delay(5);
  }
}

 
