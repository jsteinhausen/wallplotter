/*
 main 
programme central
 */

#include <Arduino.h>
#include <Servo.h>

#define LED_RED   9 //R
#define LED_GREEN 10 //G
#define LED_BLUE  11 //B

#define SWITCH_INPUT 2 // definir le pin d'entrée du Switch 


int previousSwitchState = LOW; // Déclarez une variable pour suivre l'état précédent de l'interrupteur

Servo monServo; //Déclaration de l'objet monServo

void setup()
{  
// initialisation du matériel et des paramètres de la carte Arduino

monServo.attach(12); // connecte le servo sur le pin 12
pinMode(SWITCH_INPUT, INPUT);// Initialisez le port en entrée pour l'état du switch

// Initialise les pins de la LED RGB en sortie
pinMode(LED_RED, OUTPUT);
pinMode(LED_GREEN, OUTPUT);
pinMode(LED_BLUE, OUTPUT);


Serial.begin(115200);// Initialiser le tact rate pour la communication UART

}
void loop(){

  int currentSwitchState = digitalRead(SWITCH_INPUT); // Lire l'état de l'interrupteur

  if (currentSwitchState != previousSwitchState) { // Si l'état de l'interrupteur a changé depuis la dernière itération
    if (currentSwitchState == LOW) {  // Si l'interrupteur est enfoncé (LOW), démarrez la fonction
      startFunction();
    }
    else if (currentSwitchState == HIGH) {// Si l'interrupteur est relâché (HIGH), arrêtez la fonction
      stopFunction();
    }
  }

  previousSwitchState = currentSwitchState;// Mettez à jour l'état précédent de l'interrupteur


}