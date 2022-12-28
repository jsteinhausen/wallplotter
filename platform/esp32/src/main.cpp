/*
 main 
programme central
 */

#include <Arduino.h>
#include <Servo.h>     // bibliothèque pour utiliser les Servos 
#include <TimerOne.h>  // bibliothèque pour utiliser les timers

#define LED_RED   9 //R
#define LED_GREEN 10 //G
#define LED_BLUE  11 //B

#define SWITCH_INPUT 2 // definir le pin d'entrée du Switch 


int previousSwitchState = LOW; // Déclarez une variable pour suivre l'état précédent de l'interrupteur

Servo monServo; //Déclaration de l'objet monServo
StepperController controller; //Déclaration du controller des steppers

void setup()
{  
// initialisation du matériel et des paramètres de la carte Arduino

monServo.attach(12); // connecte le servo sur le pin 12
pinMode(SWITCH_INPUT, INPUT);// Initialisez le port en entrée pour l'état du switch

// Initialise les pins de la LED RGB en sortie
pinMode(LED_RED, OUTPUT);
pinMode(LED_GREEN, OUTPUT);
pinMode(LED_BLUE, OUTPUT);

  Timer1.initialize(1000);  // initialiser le timer avec une période de 1 seconde
  Timer1.attachInterrupt(stateFunction);  // attacher la fonction maMethode à l'interruption du timer

StepperController controller;
}
void loop(){

  int currentSwitchState = digitalRead(SWITCH_INPUT); // Lire l'état de l'interrupteur

  if (currentSwitchState != previousSwitchState) { // Si l'état de l'interrupteur a changé depuis la dernière itération
    if (currentSwitchState == LOW) {  // Si l'interrupteur est enfoncé (LOW), démarrez la fonction
      startFunction();
      controller.enableMotors();
    }
    else if (currentSwitchState == HIGH) {// Si l'interrupteur est relâché (HIGH), arrêtez la fonction
      stopFunction();
      controller.stopMotors();
    }
  }

  previousSwitchState = currentSwitchState;// Mettez à jour l'état précédent de l'interrupteur


}