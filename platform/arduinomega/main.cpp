/*
Main program of the Arduino Mega
*/

#include <TimerOne.h> // Librairie for the Timers 

Timer1.initialize(1000);  // initialize timer 1 with a period of 1 second
Timer1.attachInterrupt(get_UART_com());  // attach get_UART_com to the intterupt of the Timer1


void setup(){

  // initialization of the hardware and the parameters of the Arduino board
  Serial.begin(115200); // Initialize the tact rate for UART communication
}

void loop(){

  get_UART_com();
  if (Serial.available() > 0){
    String start = Serial.readString();
    if(start == "return_home"){
      return_home();
    }
    if(){
      // Tipp the rest of the loop programm 
    }
  }
}


/*-----------------------------------------------------------Methode*------------------------------------------------*/



/*stepper_Stop is a method that reads the UART communication coming from the Arduino Mega. According to the messages communicated, 
it locks or unlocks the steppers via pin 8 (enpin). HIGH state corresponds to the activation of the steppers, LOW state to the deactivation*/

void stepper_Stop(){

if (Serial.available() > 0) {
  
    // Read the string sent by the ESP32
    String message = Serial.readString();
     if (message == "stop") {
    // lock the steppers
    enpin = HIGH; //pin 8 is HIGH == Disabel 
    }
    else if(message == "enable"){
    // unlock the steppers 
    enpin = LOW; // pin 8 is LOW == Enabel
    }
  }
}


/*get_UART_com is a Method which sends a "get_state" command on the UART communication channel to obtain the communication state.
 It is called every second by the Timer1 */

void get_UART_com(){

  Serial.println("get_state"); //get the state variable
}

