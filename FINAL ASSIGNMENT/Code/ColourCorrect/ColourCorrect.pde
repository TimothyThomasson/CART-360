// Timothy Thomasson - Colour Correct 
// This project makes use of the Minim Library and follows the AdaFruit tutorial located at ________ 
// for serial communication with Arduino. 


//-----------------------------------------------------------------------------------------------------//

//Import LIBS etc
import processing.serial.*;
import java.awt.datatransfer.*;
import java.awt.Toolkit;
import processing.video.*;
import processing.sound.*;

// import everything necessary to make sound.
import ddf.minim.*;
import ddf.minim.ugens.*;

// create all of the variables that will need to be accessed in
// more than one methods (setup(), draw(), stop()).

//objects
Minim minim;
AudioOutput out;
Oscil  wave;

// freq object for minim lib
Frequency  currentFreq;

Serial port;
Movie myMovie;
SoundFile file;

//bool to check if colour is on sensor
Boolean isColourPresent = false; 

// Vars for SERIAL stuff
String buff = "";
int wRed, wGreen, wBlue, wClear;
String hexColor = "ffffff";

void setup() {

  // initialize the minim and out objects
  minim = new Minim(this);
  out   = minim.getLineOut();

  currentFreq = Frequency.ofPitch( "A4" );
  wave = new Oscil( currentFreq, 0.6f, Waves.TRIANGLE );

  wave.patch( out );


  fullScreen();
  background(0);

  //serial 
  port = new Serial(this, "COM4", 9600);

}



void draw() {

  //Setup window  video

  checkForColour();

  if (isColourPresent) {
    drawPoints();
  }

  //Sound
  currentFreq = Frequency.ofHertz( 75 + (wRed));
  wave.setFrequency( currentFreq );

  // check for serial, and process
  while (port.available() > 0) {
    serialEvent(port.read());
  }
}

void movieEvent(Movie m) {
  m.read();
}


void checkForColour() {
  if (wClear >= 203) {
    isColourPresent = true;
  } else {
    isColourPresent = false;
  }
}

//void changeSoundWave() {
//  if (isColourPresent){
//   file.rate(wRed/20);
//  } else {
//   file.rate(1.0); 
//  }
//}


/////// Drawing Circ & Making Sound

int angle = 0;
int r = 0;

void drawPoints() {

  rectMode(CENTER);
  //float posX = (random(width));
  //float posY = (random(height));

  if (isColourPresent) {


    noStroke();
    fill(wRed, wGreen, wBlue);

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
  }
}


/////////////////////////////////////////////////////////////////////


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
      wClear = Integer.parseInt(val.trim());
    } else { 
      return;
    }

    if (cRed >=0) {
      String val = buff.substring(cRed+3);
      val = val.split("\t")[0]; 
      wRed = Integer.parseInt(val.trim());
    } else { 
      return;
    }

    if (cGreen >=0) {
      String val = buff.substring(cGreen+3);
      val = val.split("\t")[0]; 
      wGreen = Integer.parseInt(val.trim());
    } else { 
      return;
    }

    if (cBlue >=0) {
      String val = buff.substring(cBlue+3);
      val = val.split("\t")[0]; 
      wBlue = Integer.parseInt(val.trim());
    } else { 
      return;
    }
    
    print("Red: "); 
    print(wRed);
    print("\tGrn: "); 
    print(wGreen);
    print("\tBlue: "); 
    print(wBlue);
    print("\tClr: "); 
    println(wClear);
    
    wRed *= 255; 
    wRed /= wClear;
    wGreen *= 255; 
    wGreen /= wClear; 
    wBlue *= 255; 
    wBlue /= wClear; 

    hexColor = hex(color(wRed, wGreen, wBlue), 6);
    println(hexColor);
    buff = "";
  }
}