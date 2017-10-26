  const int X_PIN = 5;    //accelerometer data pins
  const int Y_PIN = 3;
  //const int Z_PIN = 3;

  int oldXValue = 0;
  int newXValue = 0;
  int oldXValue2 = 0;
  int newXValue2 = 0;

  int runningXValue;    //un-smoothed, direct accelerometer data
  int runningYValue;
  //int runningZValue;

  int firstXValue;      //value used for calibration
  int firstYValue;
  //int firstZValue;

  const int SAMPLE_SIZE = 10;
  int nextCount = 0;

  int averageXVal;      //running average accelerometer data
  int averageYVal;
  //int averageZVal;

  int runningXArray[SAMPLE_SIZE];   //buffer arrays used for smoothing
  int runningYArray[SAMPLE_SIZE];
  //int runningZArray[SAMPLE_SIZE];
  
  
void setup() {
  // put your setup code here, to run once:
  
  pinMode(5, INPUT);
  pinMode(3, INPUT);
  //pinMode(2, INPUT);
  //pinMode(6, OUTPUT);

  firstXValue = analogRead(X_PIN);    //used for calibration
  firstYValue = analogRead(Y_PIN);
  //firstZValue = analogRead(Z_PIN);

  //analogReference(EXTERNAL);

  Serial.begin(9600);
}

void loop() {
  calcAccell();       //takes in accelerometer data
  
  runningAverage();   //smooths accelerometer data
  
  serialPrint();      //prints data for use in processing
}
void calcAccell(){
  runningXValue = analogRead(X_PIN) - firstXValue;    //scales the accelerometer data in relation to first value read
  runningYValue = analogRead(Y_PIN) - firstYValue;
  //runningZValue = analogRead(Z_PIN) - firstZValue;
}

void runningAverage()
{
  int sampleTotal = 0;
  int rawXSenseVal = runningXValue;
  int rawYSenseVal = runningYValue;
  //int rawZSenseVal = runningZValue;
  
  runningXArray[nextCount] = rawXSenseVal;
  runningYArray[nextCount] = rawYSenseVal;
  //runningZArray[nextCount] = rawZSenseVal;

  nextCount++;
  if(nextCount >= SAMPLE_SIZE){
    nextCount = 0;
  }

  int currentXSum = 0;
  int currentYSum = 0;
  //int currentZSum = 0;
  
  for(int i = 0; i < SAMPLE_SIZE; i++){
    currentXSum += runningXArray[i];
    currentYSum += runningYArray[i];
    //currentZSum += runningZArray[i];
  }

  averageXVal = (currentXSum / SAMPLE_SIZE);
  averageYVal = (currentYSum / SAMPLE_SIZE);
  //averageZVal = (currentZSum / SAMPLE_SIZE);

  //Serial.print("averageVal = ");
  
}

void serialPrint(){
  //Serial.print(runningXValue);
  
  //Serial.print("\t");
  
  Serial.print((float)averageXVal, 2);
  
  Serial.print(" ");
  
  //Serial.print(runningYValue);

  //Serial.print("\t");
  
  Serial.println((float)averageYVal, 2);
  
  //Serial.print("\t");
  
  //Serial.print(runningZValue);

  //Serial.print("\t");
  
  //Serial.print(averageZVal);
  
  //Serial.println("\t");
}
