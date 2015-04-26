//Spectrometer_LED

int inPinRed = 3; // red button
int inPinBlack = 2; // black button
int redState; 
int blackState;
int val = 0; 
int analogPin = 1;
int sensorValue = 0; // ADC value of photodiode
int red = 6;
int yellow = 7;
int ir = 8;
int blue = 9;
int orange = 10;
int pinArray[] = {12, 11, 10, 9, 8}; // pin address array
int count = 0;
int sampleValue[5]; // sample array value

int learnedValues[10][5];

//----------------------------------
//Store Our Known Object Values HERE
//Update This as We LEARN
//EXAMPLE of Stored Data:
//int apple[] = {XX, XXXX, XXX, XXX, XXX}; database values for apple
//int windexValue;
//int appleTemp = 0;
// create the above for multiple items
//---------------------------------

void setup() {
  Serial.begin(19200);
  pinMode(inPinRed, INPUT);
  pinMode(inPinBlack, INPUT);
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(ir, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(orange, OUTPUT);
  for (count=0;count<4;count++)
    pinMode(pinArray[count], OUTPUT);
}


void loop() {
  delay(50);
  for (;;) {
    redState = digitalRead(inPinRed); //IDENTIFY MODE
    blackState = digitalRead(inPinBlack); //LEARN MODE
    if (redState == LOW) {//red button pressed
      goto Identify; //jump to identiy subroutine
    }
    if (blackState == LOW) { //black button pressed
      goto Learn; //jump to learn subroutine
    }
  }
        
  
  Learn: 
  //learn subroutine
  //need to store the values here and prompt user to type name of device
  // Output format: IR, Red, Yellow, Green, Blue
  
  Serial.println("========== Learn Mode ==========");
  Serial.print("{");
  for (count=0;count<5;count++) {
    digitalWrite(pinArray[count], HIGH); //turn the LEDs on sequentially
    delay(100); //wait for stabilization
    sensorValue = analogRead(analogPin); //read the value from the sensor 

    Serial.print(sensorValue);
    if (count < 4) {
      Serial.print(", ");
    }

    
    // Serial.print(sensorValue);
    // Serial.print("-");
    // delay(30);
    // if (count == 0) {
    //   Serial.print("Red  ");
    // }
    // else if (count == 1) {
    //   Serial.print("Yellow  ");
    // }
    // else if (count == 2) {
    //   Serial.print("IR  ");
    // }
    // else if (count == 3) {
    //   Serial.print("Blue  ");
    // }
    // else if (count == 4) {
    //   Serial.print("Orange  ");
    // }
      
    delay(20);
    
    digitalWrite(pinArray[count], LOW); //turn LEDs off
    delay(100);
  }
  Serial.println("}");
  
  for (;;) {
    redState = digitalRead(inPinRed);
    blackState = digitalRead(inPinBlack);
    if (redState == LOW) {
      goto Identify;
    }
    if (blackState == LOW) {
      goto Learn;
    }
  }
  
  
  
  
  Identify: 
  // identify subroutine
  
  Serial.print("========== Identify Mode ==========");
  delay(500);
 
  for (count=0;count<5;count++) { //load the matrix with 5 color values
    digitalWrite(pinArray[count], HIGH); //turn on LED
    delay(100); //wait for LED to stabalize
    sensorValue = analogRead(analogPin);
    sampleValue[count] = sensorValue; //store value in matrix
    digitalWrite(pinArray[count], LOW); //turn off LED
  }

  double smallestDistance = 1000000.0;
  int smallestIndex = -1;
  for (count = 0; count < 10; count++) {  // Number of learned objects
    int dim0 = sampleValue[0] - learnedValues[count][0];
    int dim1 = sampleValue[1] - learnedValues[count][1];
    int dim2 = sampleValue[2] - learnedValues[count][2];
    int dim3 = sampleValue[3] - learnedValues[count][3];
    int dim4 = sampleValue[4] - learnedValues[count][4];
    int distSq = dim0 * dim0 + dim1 * dim1 + dim2 * dim2 + dim3 * dim3 + dim4 * dim4;
    double result = sqrt((double)distSq);
    if (result < smallestDistance) {
      smallestDistance = result;
      smallestIndex = count;
    }
  }
  Serial.print("Identified as object ");
  Serial.println(smallestIndex);
  
  
  for (;;) {
    redState = digitalRead(inPinRed);
    blackState = digitalRead(inPinBlack);
    if (redState == LOW) {
      goto Identify;
    }
    if (blackState == LOW) {
      goto Learn;
    }
  }
}