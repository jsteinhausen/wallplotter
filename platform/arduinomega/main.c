/*
Programme principal de l'arduinomega
*/
#include <TimerOne.h>  // bibliothèque pour utiliser les timers


void setup(){
  Timer1.initialize(1000);  // initialiser le timer avec une période de 1 seconde
  Timer1.attachInterrupt(stateFunction);  // attacher la fonction maMethode à l'interruption du timer

  Serial.begin(115200);// Initialiser le tact rate pour la communication UART 
}
void loop(){

}

/*
 * stateFunction
 * vérifie l'état du robot :
 * dispose de 3 états moteur à l'arrêt, moteur en mouvement et message en cours de reception. 
 * Elle est appellée toute les secondes dans la fonction principale pour afficer un état en continu 
 */
void stateFunction(){
    // Envoie une requête toutes les secondes pour obtenir la variable état
  Serial.println("get_state");

  // Si un message est disponible sur la ligne UART, lit-le
  if (Serial.available()) {
    String message = Serial.readString();

    if (message == "stopped") {
      // Moteur à l'arrêt : allume la LED rouge
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_BLUE, LOW);
    }
    else if (message == "moving") {
      // Moteur en mouvement : allume la LED verte
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_BLUE, LOW);
    }
    else if (message == "received") {
      // Arduino reçoit un message : allume la LED bleue
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_BLUE, HIGH);
    }
  }
}