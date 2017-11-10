/*----------------------------------------------------------------------------*/
/*------------------------------ SHIFT REGISTER ------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------      IMPLEMENT THE SHIFT REGISTER FUNCTIONALITY        ----------*/
/*----------          ******** ATTRIBUTE YOUR CODE ********         ----------*/
/*----------------------------------------------------------------------------*/

#ifndef SHIFTREGISTER_H_
#define SHIFTREGISTER_H_

#include "Constants.h"

/* SHIFT REGISTER IC PIN OUTS
        __
  Q1 -|    |- VCC
  Q2 -|    |- Q0
  Q3 -|    |- DS
  Q4 -|    |- OE
  Q5 -|    |- ST_CP
  Q6 -|    |- SH_CP
  Q7 -|    |- MR
  GND -| __ |- Q'
  Key:
  Q0 - Q7: Parallel Out Pins
  Q': Cascade Pin
  DS: Serial Data In Pin
  OE: Output Enable (GND)
  ST_CP: Latch Pin
  SH_CP: Clock Pin
  MR: Master Reset (VCC)
*/

/* PINS FOR SHIFT REG */
// ST_CP of 74HC595
#define LATCH_PIN 6
// SH_CP of 74HC595
#define CLOCK_PIN 8
// DS of 74HC595
#define DATA_PIN 7

/* CONSTANT FOR EMPTY*/
#define EMPTY B00000000

/* DEFINE AND INITIALIZE THE ARRAY WITH THE NECESSARY VALUES */
// bit array for use with 7-segment display
byte hexArray[16] =
{
  B10000001,    //0
  B11110011,    //1
  B01001001,    //2
  B01100001,    //3
  B00110011,    //4
  B00100101,    //5
  B00000101,    //6
  B11110101,    //7
  B00000001,    //8
  B00110001,    //9
  B00010001,    //A
  B00000111,    //B
  B10001101,    //C
  B01000011,    //D
  B00001101,    //E
  B00011101     //F
};

void sendToShiftRegister(int pot)
{
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN,  MSBFIRST, hexArray[pot]);        //shifts data out
  digitalWrite(LATCH_PIN, HIGH);      //opens register for communication
}

void blinkSevenSegmentDisplay() {

  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111);
  digitalWrite(LATCH_PIN, HIGH);

  delay(DURATION);

  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111110);
  digitalWrite(LATCH_PIN, HIGH);

  delay(DURATION);

}

void setupShiftRegister() {
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
}

int getAnalog() {
  return analogRead(POT_PIN) / 64;      // translates range from 0 - 1023 to 0 - 15
}

#endif /* SHIFTREGISTER_H_ */
