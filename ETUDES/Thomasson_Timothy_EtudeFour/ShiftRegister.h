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
#define EMPTY B00000001

/* DEFINE AND INITIALIZE THE ARRAY WITH THE NECESSARY VALUES */
// Hex Character from 0 - F
byte hexArray[16] =
{
  // you have to implement the necessary values for each element
  
  //1 
  B10000001,
  //2
  B11110011,
  //3
  B01001001,
  //4
  B00110011,
  //5
  00100101,
  //6
  00000101,
  //7
  1111000,
  //8
  00000001,
  
  
};

void setupShiftRegister() {
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  }
  
/******************sendToShiftRegister *******************************
TODO:: IMPLEMENT THE FUNCTIONALITY TO SEND THE CORRECT DATA TO 
SHIFT REG - BASED ON THE POT VAL
********************************************************************/
void sendToShiftRegister(int pot)
{
  // IMPLEMENT

  
}

/******************READ FROM POT*********************************
TO DO:: GET VALUE FROM POT AND ENSURE THE VALUE RETURNED IS A VALID VALUE 
********************************************************************/
int getAnalog() {

  //IMPLEMENT
}

#endif /* SHIFTREGISTER_H_ */
