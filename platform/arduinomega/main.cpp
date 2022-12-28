/*
Programme principal de l'arduinomega
*/

#include <TimerOne.h>
#include <Stepper.h>

  Timer1.initialize(1000);  // initialiser le timer avec une période de 1 seconde
  Timer1.attachInterrupt(getEmergencyMessage);  // attacher la fonction maMethode à l'interruption du timer


  const int stepsPerRevolution = 200;
  Stepper motor1(stepsPerRevolution, 8, 9, 10, 11);
  Stepper motor2(stepsPerRevolution, 12, 13, 14, 15);
  Stepper motor3(stepsPerRevolution, 16, 17, 18, 19);
  Stepper motor4(stepsPerRevolution, 20, 21, 22, 23);

void setup(){

  Serial.begin(9600);// Initialiser le tact rate pour la communication UART 
}
void loop(){

}

void getEmergencyMessage(){
    Serial.println("get_state"); //obtenir la variable état 

if (Serial.available() > 0) {
      // Lecture du string envoyé par l'ESP
    String message = Serial.readString();
     if (message == "stop") {
    // Arrêt des steppers
    motor1.disable();
    motor2.disable();
    motor3.disable();
    motor4.disable();
    }
    else if(message == "enable"){
    //restart des steppers
    motor1.enable();
    motor2.enable();
    motor3.enable();
    motor4.enable();
    }
  }
}