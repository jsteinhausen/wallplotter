/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#include "Arduino.h" 

// Set LED_BUILTIN if it is not defined by Arduino framework
#define LED_BUILTIN 2
#define LED_RED   9
#define LED_GREEN 10
#define LED_BLUE  11
#define SWITCH_INPUT 2

// Déclarez une variable pour suivre l'état précédent de l'interrupteur
int previousSwitchState = LOW;

void setup()
{  
// initialisation du matériel et des paramètres de la carte Arduino

pinMode(LED_BUILTIN, OUTPUT); // initialize LED digital pin as an output.
digitalWrite(LED_BUILTIN, HIGH); // turn the LED on ( HIGH is the voltage level)
delay(1000);// wait for a second
digitalWrite(LED_BUILTIN, LOW);// turn the LED off by making the voltage LOW
delay(1000);// wait for a second

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