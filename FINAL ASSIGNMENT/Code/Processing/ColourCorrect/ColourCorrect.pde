// Timothy Thomasson - Colour Correct 
// This project makes use of the Minim Library and follows the AdaFruit tutorial located at https://learn.adafruit.com/adafruit-color-sensors/use-it-with-processing 
// for serial communication with Arduino. 

//-----------------------------------------------------------------------------------------------------//

//Import LIBS
import processing.serial.*;
import java.awt.datatransfer.*;
import java.awt.Toolkit;
import processing.video.*;
import processing.sound.*;

// import everything necessary to make sound.
import ddf.minim.*;
import ddf.minim.ugens.*;

//create objects
Minim minim;
AudioOutput out;
Oscil  wave;

// freq object for minim libs
Frequency  currentFreq;

Serial port;
Movie myMovie;
SoundFile file;

//bool to check if colour is on sensor
Boolean isColourPresent = false; 

// Vars for SERIAL stuff
String buff = "";
int red, green, blue, clear;
String hexColor = "ffffff";

//vars for drawing 
int angle = 0;
int r = 0;

void setup() {

  fullScreen();
  background(0);

  // initialize the minim and other objects
  minim = new Minim(this);
  out   = minim.getLineOut();

  //create wave
  currentFreq = Frequency.ofPitch( "A4" );
  wave = new Oscil( currentFreq, 0.6f, Waves.TRIANGLE );
  wave.patch( out );

  //serial
  port = new Serial(this, "COM4", 9600);
  //until new line
  port.bufferUntil('\n');
}


void draw() {

  checkForColour();

  if (isColourPresent) {
    drawPoints();
  }

  // check for serial
  while (port.available() > 0) {
    serialEvent(port.read());
  }
}


void checkForColour() {
  if (clear >= 203) {
    isColourPresent = true;
  } else {
    isColourPresent = false;
  }
}


// Drawing Circ
void drawPoints() {
  
  String fileName = "canvas.png";
  int fileNumber = 0;

  rectMode(CENTER);

  if (isColourPresent) {

    noStroke();
    fill(red, green, blue);

    float x = width/2 + cos(radians(angle)) * r;
    float y = height/2 + sin(radians(angle)) * r;

    rect(x, y, 5, 5);
    angle += 5;
    delay(50);

    if (angle >= 360) {
      r += 10;
      angle = 0;
      println (r);
    }
  } 
  if (r >= 1920) {
    background (0);
    r = 0;
    angle = 0;
    fileNumber++;
    save(fileName+fileNumber);
  }
}


void manipulateFreq() {
  currentFreq = Frequency.ofHertz( 75 + (red));
  wave.setFrequency( currentFreq );
}

// for getting rgb vals only
void getNeededRGBVals(Serial p) {

  String input = p.readStringUntil('\n'); 
  String test = input.trim();
  int rgbVals [] = int(split(test, ','));

  print(rgbVals[0]);
  print(rgbVals[1]);
  print(rgbVals[2]);
  print(rgbVals[3]);
  print(rgbVals[4]);
}




///////////////////////////////////////////////////////////////////// from Adafruit breakout code avialble in Library: 
//https://learn.adafruit.com/adafruit-color-sensors/use-it-with-processing


void serialEvent(int serial) {
  if (serial != '\n') {
    buff += char(serial);
  } else {
    //println(buff);

    int cRed = buff.indexOf("R");
    int cGreen = buff.indexOf("G");
    int cBlue = buff.indexOf("B");
    int clear = buff.indexOf("C");
    if (clear >=0) {
      String val = buff.substring(clear+3);
      val = val.split("\t")[0]; 
      clear = Integer.parseInt(val.trim());
    } else { 
      return;
    }

    if (cRed >=0) {
      String val = buff.substring(cRed+3);
      val = val.split("\t")[0]; 
      red = Integer.parseInt(val.trim());
    } else { 
      return;
    }

    if (cGreen >=0) {
      String val = buff.substring(cGreen+3);
      val = val.split("\t")[0]; 
      green = Integer.parseInt(val.trim());
    } else { 
      return;
    }

    if (cBlue >=0) {
      String val = buff.substring(cBlue+3);
      val = val.split("\t")[0]; 
      blue = Integer.parseInt(val.trim());
    } else { 
      return;
    }

    print("Red: "); 
    print(red);
    print("\tGrn: "); 
    print(green);
    print("\tBlue: "); 
    print(blue);
    print("\tClr: "); 
    println(clear);

    red *= 255; 
    red /= clear;
    green *= 255; 
    green /= clear; 
    blue *= 255; 
    blue /= clear; 

    hexColor = hex(color(red, green, blue), 6);
    println(hexColor);
    buff = "";
  }
}