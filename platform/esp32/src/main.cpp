/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#include "Arduino.h"
// Set LED_BUILTIN if it is not defined by Arduino framework
#define LED_BUILTIN 2
// Déclarez une variable pour suivre l'état précédent de l'interrupteur
int previousSwitchState = LOW;

void setup()
{    // turn the LED on (HIGH is the voltage level)
// initialize LED digital pin as an output.
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    // wait for a second
    delay(1000);
    // turn the LED off by making the voltage LOW
    digitalWrite(LED_BUILTIN, LOW);
    // wait for a second
    delay(1000);
    // Initialisez le port en entrée
   pinMode(2, INPUT);
}
void loop(){
  // Lisez l'état de l'interrupteur
  int currentSwitchState = digitalRead(2);

  // Si l'état de l'interrupteur a changé depuis la dernière itération
  if (currentSwitchState != previousSwitchState) {
    // Si l'interrupteur est enfoncé (LOW), démarrez la fonction
    if (currentSwitchState == LOW) {
      startFunction();
    }
    // Si l'interrupteur est relâché (HIGH), arrêtez la fonction
    else if (currentSwitchState == HIGH) {
      stopFunction();
    }
  }

  // Mettez à jour l'état précédent de l'interrupteur
  previousSwitchState = currentSwitchState;
}

void startFunction() {
  // Insérez ici le code à exécuter lorsque la fonction est démarrée
}

void stopFunction() {
  // Insérez ici le code à exécuter lorsque la fonction est arrêtée
}