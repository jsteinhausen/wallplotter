
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
const int numberOfSensors=4;
const int myPins[] = {12, 14, 27, 26};
const int limitValue=1000;


int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)

void setup() {
    // initialize serial communications at 9600 bps:
    Serial.begin(9600);
}
int lenght(int myInts[numberOfSensors]){
    return sizeof(myInts[numberOfSensors])/ sizeof(myInts[0]);
}
int readQD(int QRE113_Pin){
    //Returns value from QRE1113
    //Lower numbers mean more refleacive
    //More than 300 means nothing was reflected
    pinMode(QRE113_Pin,OUTPUT);
    digitalWrite(QRE113_Pin, HIGH);
    delayMicroseconds(10);
    pinMode(QRE113_Pin,INPUT);
    long time=micros();
    while (digitalRead(QRE113_Pin)==HIGH && micros()-time < 3000);
    int diff=micros()-time;
    Serial.print("sensor = ");
    Serial.println(diff);
    return diff;
}

bool lineDetected(){
    for(int i=0;i< numberOfSensors;i++){
        int value=readQD(i);
        Serial.println(value);
        if(value>limitValue){
            Serial.println("Line detected");
            return true;
            break;
        }
    }
    return false;
}
void loop() {
    // read the analog in value:
    bool isLine=lineDetected();
    // wait 2 milliseconds before the next loop for the analog-to-digital
    // converter to settle after the last reading:
    delay(100);
}



