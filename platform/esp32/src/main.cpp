
/*
  Analog input, analog output, serial output

  Reads an analog input pin, maps the result to a range from 0 to 255 and uses
  the result to set the pulse width modulation (PWM) of an output pin.
  Also prints the results to the Serial Monitor.

  The circuit:
  - potentiometer connected to analog pin 0.
    Center pin of the potentiometer goes to the analog pin.
    side pins of the potentiometer go to +5V and ground
  - LED connected from digital pin 9 to ground through 220 ohm resistor

  created 29 Dec. 2008
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/AnalogInOutSerial
*/
#include "Arduino.h"

// These constants won't change. They're used to give names to the pins used:
const int numberOfSensors=10;
const int myPins[] = {12,13 ,14, 22,23,27, 26,25,32,33};
const int differenceLineValue=50;

int counter=0;
int lastSensorValues[]={0, 0, 0, 0,0,0,0,0,0,0};
int defaultSensorValues[]={0, 0, 0, 0,0,0,0,0,0,0};

//0: No Line Detected;
//1: Line Detected;
int stateLineDetection=0;


int readQD(int QRE113_Pin){
    //Returns value from QRE1113
    pinMode(QRE113_Pin,OUTPUT);
    digitalWrite(QRE113_Pin, HIGH);
    delayMicroseconds(10);
    pinMode(QRE113_Pin,INPUT);
    long time=micros();
    while (digitalRead(QRE113_Pin)==HIGH && micros()-time < 3000);
    int diff=micros()-time;
    return diff;
}

void setup() {
    // initialize serial communications at 9600 bps:
    Serial.begin(9600);
    //initialize last sensor values as a Reference
    for(int i=0;i<numberOfSensors;i++){
        lastSensorValues[i]= readQD(myPins[i]);
    }
}
int lenght(int myInts[numberOfSensors]){
    return sizeof(myInts[numberOfSensors])/ sizeof(myInts[0]);
}

int lineDetected(){
    int value=0;
    int lastPin=0;
    for(int i=0;i< numberOfSensors;i++){
        value=readQD(myPins[i]);
        Serial.print("GPIO");
        Serial.print(myPins[i]);
        Serial.print(": ");
        Serial.println(value);
        lastPin=myPins[i];
        if((value>=(lastSensorValues[i]+differenceLineValue))&stateLineDetection==0) {
            stateLineDetection = 1;
            break;
        }else if((stateLineDetection==1&value<=(lastSensorValues[i]-differenceLineValue))){
                stateLineDetection=0;
        }
        lastSensorValues[i]=value;
        }

    switch(stateLineDetection) {
        case 0:
            //Serial.println("No line detected");
            return stateLineDetection;
            break;
        case 1:
            //Debugging
            Serial.print(lastPin);
            Serial.print(": ");
            Serial.println(value);
            //Serial.println("Line detected");
            return stateLineDetection;
            break;
        default:
            return stateLineDetection;
    }
    }

void loop() {
    const int testTimeMicroS=5000000;
    const int preparationTimeS=20;
    counter++;
    Serial.print("Experiment ");
    Serial.println(counter);
    if (counter%2==1){
        Serial.println("Straight Line");
    }
    else{
        Serial.println("Curved Line");
    }

    Serial.print("You have ");
    Serial.print(preparationTimeS);
    Serial.println(" Seconds to prepare for the next Test");
    for(int i=preparationTimeS;i>=0;i--){
        Serial.print(" ");
        if (i%11==0) {
            Serial.println(i);
        }
        else{
            Serial.print(i);
        }
        //1 second delay
        delay(1000);
    }
    Serial.println("Start");

    long time=micros();
    for(int i=0;i<numberOfSensors;i++){
        defaultSensorValues[i]=lastSensorValues[i];
    }
    stateLineDetection=0;
    int onLine=0;
    while( (onLine==0)&(micros()-time<=testTimeMicroS)){
        onLine=lineDetected();
    }
    long diff=micros()-time;
    if (onLine==1){
        Serial.print("Line Detected after ");
        Serial.print(diff);
        Serial.println(" microsecondes");
    }
    else if(onLine==0){
        Serial.print("No Line Detected after ");
        Serial.print(testTimeMicroS);
        Serial.println( " microseconds");
    }
}



