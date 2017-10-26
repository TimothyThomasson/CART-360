import processing.serial.*;
import processing.sound.*;

Serial myPort;  // Create object from Serial class
String inString;
int lf = 10;

int timeNow;
int timePassed;
int startTime;


String val;     // Data received from the serial port
String xVal = " ";
String yVal = " ";

SoundFile file;
SoundFile file2;

void setup() {
  size(640, 360);
  background(255);
  
  //constructs serial port object, sets baud rate and buffer size
  String portName = Serial.list()[4]; //change the 0 to a 1 or 2 etc. to match your port
  myPort = new Serial(this, portName, 9600);
  myPort.bufferUntil(lf); 
  
  // Loads sound files from the data folder
  file = new SoundFile(this, "testSong2.mp3");
  file2 = new SoundFile(this, "testSong.mp3");
  
  //starts sound files but sets their play rate to 0
  file.play();
  file2.play();
  file2.rate(0); 
  file.rate(0);
  
  //timer used for song switching
  startTimer();
  
}      

void draw() {
  if(inString != null){                //used to avoid null pointer acceptions
    if(inString.substring(0) == "-"){  //seperates serial string data into the two accelerometer values
      xVal = val.substring(0, 5);
      yVal = val.substring(5, 9);
    }
    else{
      xVal = inString.substring(0, 4);
      yVal = inString.substring(5, 7);
    }
  }
  
  if(Math.abs(float(trim(xVal))) < 1){    //if accelerometer data has been between -1 and 1 for 5 seconds then the "idle" song is played
    if(getTimePassed() > 5000){
      file.rate(0);
      file2.rate(1);
    }
  }else{
    startTimer(); 
  }
  
  if(myPort.available() > 0){  //If data is available,
    if(inString != null && Math.abs(float(trim(xVal))) > 1){   //if the accelerometer data is not between -1 and 1 then modulate the song speed based on the data, 
      file.rate(Math.abs(float(trim(xVal))/15.00));
      file2.rate(0);
      //file.amp(map(float(yVal), 0, 10, 0, 1));
      print(Math.abs(float(trim(xVal))/15.00) + "  ");
    }
  }else{                     //else keep the rate at normal
    file.rate(1);
  }
  //println(xVal + " " + yVal);
  //println(val);
  print(inString + "  ");
  println(getTimePassed());
}
void serialEvent(Serial p) { 
  inString = p.readString(); 
}
void startTimer(){
  startTime = millis();
}
int getTimePassed(){
 return (millis() - startTime); 
}