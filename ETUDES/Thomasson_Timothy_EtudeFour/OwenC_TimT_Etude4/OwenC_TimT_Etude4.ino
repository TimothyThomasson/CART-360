

/**********************OWEN COOLIDGE && TIMOTHY THOMASSON******************/


/**********************ETUDE 4 CART 360 2017*******************************
   1/ As in Etude 3, in Etude 4 you will still Play & Record Notes and capture the associated Note Duration.
   Additionally, in the record() , Pauses between Notes are also captured - you need to implement.
   2/In Playback Mode, you will NOW have the option to Store the Pattern of Notes you just recorded by pressing the Store Button -
   3/ You will be able to store up to 16 Patterns (Hardcoded Limit).
   4/ Loop Mode has been Deprecated and has been replaced by the new Retrieve Mode ( retrieveMode() ).
   In Retrieve Mode, you will be able to Select a Stored Pattern and Retrieve it
   - the Stored Pattern Number will be displayed on the 7-Segment Display.
   To Play the Stored Pattern, you will Press the Store Button (Confirm Selection).
   The Potentiometer, Shift Register and 7-Segment Display work in concert to support your intention:
   In Retrieve Mode, the Potentiometer selects Pattern ID -> Sends Pattern ID to Shift Register -> Shift Register to 7-Segment Display.
*/

#include "Constants.h"
#include "InterruptHandler.h"
#include "ShiftRegister.h"
#include "TestPattern.h"
#include "Timer.h"

/******** VARIABLES *****************************************************/
// counter for how many notes we have
int countNotes = 0;
// array to hold the notes played (for record/play mode)
int notes [MAX_NOTES];
int mode = 0; //

/******** VARIABLES FOR ETUDE 3 *****************************************************/
// NEW FOR ETUDE 3: array to hold durations
// will be a parallel array to the notes array -
// meaning that at index 0 of notes[] is stored the 1st frequency, and at index 0 of
// the durations array  is stored the 1st duration for THAT note....
int durations [MAX_NOTES];
int durations_In_Between[MAX_NOTES];

// boolean to determine if a note button was being pressed:
boolean activeNoteButton = false;

// keep track of the active frequency
int activeFrequency = 0;

// var to hold the note pressed
int testNote;

// var to hold a reference to the timer object used for finding the note durations
Timer t1;

// ** NEW :: this timer object YOU will need to use  for finding pauses BETWEEN notes
Timer t_between;

/**********************SETUP() DO NOT CHANGE*******************************/
// Declare pin mode for the single digital input
void setup()
{
  Serial.begin(9600);

  // INTERRUPT HANDLER
  setupInterruptHandler();

  // SHIFT REGISTER
  setupShiftRegister();
}


/**********************LOOP() *******************************
   INSTRUCTIONS:
   The loop establishes the flow of the program:
   If the mode Button has been pressed (modeChangeInterruptRequest boolean == true) then:
   1: chooseMode():  &&&
   2. YOU need to ensure that the  7-SEGMENT DISPLAY is off.
   Then  - regardless of if the button is pressed:
   3: setRGB(): will set the color of the RGB LED based on the value of the mode variable
   4: selectMode(): will determine which function to call based on the value of the mode variable
**************************************************************************/
void loop()
{
  if (modeChangeInterruptRequest)
  {
    chooseMode();
    delay(DURATION * 2);
    modeChangeInterruptRequest = ISFALSE;
  }
  setRGB();
  selectMode();
}

/******************CHOOSEMODE() -NOTHING TO CHANGE *********************************
   INSTRUCTIONS:
   Read the value from the Button (linked to the BUTTON_MODE_PIN) and
   if is being pressed then change the mode variable.
   REMEMBER:
   mode == 0 is reset
   mode == 1 is live
   mode == 2 is record
   mode == 3 is play
   mode == 4 is retrieve mode
   Every time the user presses the button, the program will go to the next mode,
   once it reaches 4, it should go back to mode == 0.
   (i.e. if mode ==2 and we press, then mode ==3) ...
**************************************************************************/
void chooseMode() {
  if (mode < 4) mode++;
  else mode = 0;
}

/**********************SELECTMODE() - NOTHING TO CHANGE *******************************
   INSTRUCTIONS:
   There is NO NEED to change this function - it selects WHICH function should run based on the mode variable
   There are 4 possibilities
   1: reset(): this function will reset any arrays etc, and will do nothing else
   2: live(): this function will play the corresponding notes
   to the user pressing the respective buttons.
   NOTE:: the notes played are NOT stored
   3: recordWithDuration(): this function will play the corresponding notes
   to the user pressing the respective buttons
   AND will STORE up to 16 consecutive notes in an array.
   4: playWithDuration(): this function will playback any notes stored in the array that were recorded
   in the previous mode AND allow the user to store the pattern.
   5: retrieve():
******************************************************************************/
void selectMode()
{
  if (mode == 0) {
    reset();
  }
  else if (mode == 1) {
    live();
  }
  else if (mode == 2) {
    recordWithDuration();
  }
  else if (mode == 3) {
    playWithDuration();
  }
  else if (mode == 4) {
    retreiveMode();
  }
}
/******************RESET() **************************************
   INSTRUCTIONS:
   this function should ensure that any notes recorded are no longer available
   ALSO: to IMPLEMENT: turn off the SEVEN SEGMENT DISPLAY
**************************************************************************/
void reset() {
  countNotes = 0;
  noTone(BUZZER_PIN);
  storeOnce = ISFALSE;
  digitalWrite(LATCH_PIN, LOW);
}
/******************LIVE()- NOTHING TO DO **************************************
   INSTRUCTIONS:
   this function will play the corresponding notes
   to the user pressing the respective buttons
   NOTE:: the notes played are NOT stored
   SO: you need read in the input from the analog input (linked to the button-resistor ladder combo)
   ALSO - get the value of the photo cell and combine it with the note frequency
   THEN - output the note to the buzzer using the tone() function, as 3rd argument
   make the duration value == to 100
**************************************************************************/
void live() {
  if (analogRead(NOTE_IN_PIN) > 8) {
    tone(BUZZER_PIN, analogRead(NOTE_IN_PIN), 100);
  }
  if (storePatternInterruptRequest == ISTRUE)
  {
    storePatternInterruptRequest = ISFALSE;
  }
}

/******************RECORDWITHDURATION(): IMPLEMENT NEW ETUDE 4 ************
   INSTRUCTIONS:
   YOU need to implement the new functionality for capturing the pauses between notes ...
   HINT: you will need to use the second timer and at some point call the  addToArrayInBetween() function which will
   update the necessary data structures.
   THERE IS NO INDICATION WHERE OR HOW YOU SHOULD DO THIS IN THIS FUNCTION

**************************************************************************/
void recordWithDuration()
{
  int theAdjuster = 8;
  if (countNotes < MAX_NOTES) {               // confirm that 16 notes have not been recorded yet
    testNote = analogRead(NOTE_IN_PIN);       // store value from button press
    /*** STATE A::: *******************************************
        IF the testNote is > min AND the timer has not reached 5 secs:
        This means we are pressing on a note button
    */
    if (testNote > theAdjuster && (t1.getTimePassed() < MAX_PRESS_TIME)) {
      /*** STATE AA::: *******************************************
          IF the boolean is false it means we have just started to press the button
          YOU now need to implement the function below which will set up all things
          required to have a new timer
      */
      if (activeNoteButton == ISFALSE) {

        if (countNotes) {               // makes sure that the silence before any notes have been played isn't stored
          addToArrayInBetween();
        }
        t1.startUpTimer();              //starts durations timer
        activeNoteButton = ISTRUE;
      }
      /*** STATE AA::: *******************************************
           IF the boolean is true it means we are continuing to press the button
      */
      else {
        t1.updateTimer();             // updates the note durations timer
        activeFrequency = testNote;
        playCurrentNote();
      }
    }
    /*** STATE B::: *******************************************
       IF the testNote is > min AND the timer HAS reached 5 secs:
       then turn OFF the buzzer.
    */
    else if (testNote > theAdjuster && (t1.getTimePassed() > MAX_PRESS_TIME)) {
      noTone(BUZZER_PIN);
    }
    /*** STATE C::: *******************************************
       IF the testNote is < min AND the boolean is true
       this means we have STOPPED pressing the button, update the arrays
    */
    else if ((testNote <= theAdjuster && activeNoteButton == ISTRUE) ) {
      /*** FUNCTION TO IMPLEMENT ***/
      t_between.startUpTimer();           //starts recording the duration of the silence
      updateArraysWithNoteAndTimings();
    }
  }
  if (storePatternInterruptRequest == ISTRUE) {  // makes sure that in record mode a pattern is not saved
    storePatternInterruptRequest = ISFALSE;

  }
}


/******************PLAYCURRENTNOTE(): *********************************
   INSTRUCTIONS:
   this function will play the current note being pressed in RecordDuration mode
**************************************************************************/
void playCurrentNote() {
  tone(BUZZER_PIN, activeFrequency, 100);
}
/******************UPDATEARRAYSWITHNOTEANDTIMINGS():SMALL CHANGE ************
   INSTRUCTIONS:
   this function handles the case where the note is no longer being pressed:
   you need to update the arrays
   reset the necessary booleans
   reset any counters
   //TO DO :: perhaps we want to start up our other timer ????
**************************************************************************/
void updateArraysWithNoteAndTimings() {
  notes[countNotes] = activeFrequency;
  durations[countNotes] = t1.getTimePassed();
  countNotes++;
  activeNoteButton = ISFALSE;
  t1.setTimePassed(0);
}
/******************addToArrayInBetween(): IMPLEMENT *********************************
   INSTRUCTIONS:
   this function will handle the case that adds the current pause into the necessary array
   You may also need to reset some variables....
  /*************************************************************************/
void addToArrayInBetween() {

  t_between.updateTimer();
  durations_In_Between[countNotes] = t_between.getTimePassed();     // Stores time inbetween

  t_between.setTimePassed(0);     // Resets time inbetween timer
}

/******************PLAYWITHDURATION(): ADAPTED FOR ETUDE 4 *******************
   INSTRUCTIONS:
   this function goes through the notes array and play back the note and
   its corresponding duration value.
   TO IMPLEMENT:: add the ability to store the current sequence playing within the if statement
**************************************************************************/
void playWithDuration() {
  int i = 0;

  while (storePatternInterruptRequest == ISFALSE && modeChangeInterruptRequest == ISFALSE ) {
    if (countNotes > 0) {
      tone(BUZZER_PIN, notes[i], durations[i]);
      delay(durations[i]);                    //delays while buzzer is playing

      delay(durations_In_Between[i]);         // delays amount inbetween respective notes

      if (i < countNotes - 1) {
        i++;

      } else {

        i = 0;
        delay(DURATION);

      }
    }
  }

  if (countNotes > 0 && storePatternInterruptRequest == ISTRUE && storeOnce == ISFALSE)
  {

    addPattern(notes, durations, durations_In_Between, countNotes);  //using addPattern() function from TestPattern class to store recored pattern

    storeOnce = ISTRUE;

    digitalWrite(LED_PIN_G, LOW);        //blinks LED to signify a pattern has been saved
    delay(100);
    digitalWrite(LED_PIN_G, HIGH);

  }
  delay(DURATION * 2);
  storePatternInterruptReset();
}
/******************HELPER STATE 0 - PARTIALLY IMPLEMENT *******************
      INSTRUCTIONS: while neither button is pressed then:
      1/ get pot value
      2/if we have do NOT have a valid index (i.e. we selected "3" but there are only 3 patterns not 4) then
      make sure the seven segment display blinks.
      3/ otherwise, make sure that the shift register sends the correct number to the seven segment display

**************************************************************************/
void selectThePatternWithPot_Stage_0_(int & shiftRef)
{
  while (storePatternInterruptRequest == ISFALSE && modeChangeInterruptRequest == ISFALSE) {

    shiftRef = getAnalog();

    if (isValidIndex(shiftRef) == ISFALSE) {  // index is not valid
      blinkSevenSegmentDisplay();
    }

    else {                                    // index is valid
      sendToShiftRegister(shiftRef);
    }
  }
}
/******************HELPER STATE 1 -PARTIALLY IMPLEMENT *********************************
      INSTRUCTIONS:
  While the modebutton is not pressed: if we have a valid index
  then:
  1/ use the TestPattern functions to retrieve the notes,
  durations and pauses for that pattern.
  2/  Then play the current pattern
**************************************************************************/
void retrieveThePatternAccordingToSelection_Stage_1(int & shiftRef)
{
  while (modeChangeInterruptRequest == ISFALSE) {       //while mode button is not pressed
    if (isValidIndex(shiftRef) == ISTRUE) {

      const int * noteArray = retrievePatternNotes(shiftRef);
      const int * durationsArray = retrievePatternDurations(shiftRef);
      const int * pausesArray =  retrievePatternPauses(shiftRef);

      int i = 0;
      countNotes = getNumberNotesOfPattern(shiftRef);

      while (storePatternInterruptRequest == ISFALSE && modeChangeInterruptRequest == ISFALSE ) {
        if (countNotes > 0) {     // makes sure that values are in the array
          tone(BUZZER_PIN, noteArray[i], durationsArray[i]);      //plays back recorded song
          delay(durations[i]);
          delay(pausesArray[i]);

          if (i < countNotes - 1) {
            i++;
          } else {
            i = 0;
            delay(DURATION);
          }
        }
      }
    }
  }
}

/******************RETRIEVEMODE(): IMPLEMENT *********************************
   INSTRUCTIONS:
   1/ while there are no patterns and the mode button is not pressed then blink the
   seven segment display for DURATION
   2/ otherwise select the pattern and prehaps retrieve it

**************************************************************************/
void retreiveMode()
{
  noTone(BUZZER_PIN);
  static int shift = -1;

  while (patternListIsEmpty() == ISTRUE && modeChangeInterruptRequest == ISFALSE) {   //while there are no patterna and mode button is not pressed
    blinkSevenSegmentDisplay();
  }

  selectThePatternWithPot_Stage_0_(shift);

  delay(DURATION * 2);

  storePatternInterruptReset();
  retrieveThePatternAccordingToSelection_Stage_1(shift);
}

/******************SETRGB() - NOTHING TO DO*********************************
   INSTRUCTIONS:
   Depending on the value of the mode variable:
   if the mode is 0 - RGB LED IS OFF
   if the mode is 1 - RGB LED IS BLUE
   if mode is 2 - RGB LED IS RED
   if mode is 3 - RGB LED IS GREEN
   if mode is 4 - RGB LED iS PURPLE
   YOU MUST USE A SWITCH CASE CONSTRUCT (NOT A SERIES OF IF / ELSE STATEMENTS
**************************************************************************/
void setRGB()
{
  switch (mode)
  {
    case 0:
      {
        analogWrite(LED_PIN_R, 0);   // Turn on the LED -R
        analogWrite(LED_PIN_G, 0);   // Turn on the LED -G
        analogWrite(LED_PIN_B, 0);   // Turn on the LED -B
        break;
      }
    case 1:
      {
        analogWrite(LED_PIN_R, 0);   // Turn on the LED -R
        analogWrite(LED_PIN_G, 0);   // Turn on the LED -G
        analogWrite(LED_PIN_B, 255);   // Turn on the LED -B
        break;
      }
    case 2:
      {
        analogWrite(LED_PIN_R, 255);   // Turn on the LED -R
        analogWrite(LED_PIN_G, 0);   // Turn on the LED -G
        analogWrite(LED_PIN_B, 0);   // Turn on the LED -B
        break;
      }
    case 3:
      {
        analogWrite(LED_PIN_R, 0);   // Turn on the LED -R
        analogWrite(LED_PIN_G, 255);   // Turn on the LED -G
        analogWrite(LED_PIN_B, 0);   // Turn on the LED -B
        break;
      }

    case 4:
      {
        analogWrite(LED_PIN_R, 125);   // Turn on the LED -R
        analogWrite(LED_PIN_G, 0);   // Turn on the LED -G
        analogWrite(LED_PIN_B, 255);   // Turn on the LED -B
        break;
      }
  }
}

/**************************************************************************
  Anwser the  questions IN THIS SPACE....

  1.The purpose behind the shift register is to put forward serial inputs and manipulate the
  on/off switches of the pins. We are using the 8 pins in the shift reg. to change the values
  in the 7 segment display

  2. To allow users to sequentially play each recorded comp. without having to go through the
  whole cycle would be to take in the value of the potentiometer when the playback button is
  pressed again to stop the replay. Then you could use this value to get the proper array.

  3. To add changes from the photocell to the composition, elements would have to be read in a
  large array based off a timer, and upon the retrieval of each element values from the photocell
  would get added.

  4.Our first  idea is to implement a third button to create a multitrack composition. The
  user would press this button, and the previously recorded track would start to play.  The
  user could then play over what was already recorded and add layers to it. Then, when this
  button is pressed again, the tracks would merge together to create a more complex composition.
  Another interesting addition to this instrument could be a third button that plays each stored
  composition back sequentially. That way you could make simple musical patterns that when linked
  together becomes more interesting and longer strings of sounds.
  /**************************************************************************/
