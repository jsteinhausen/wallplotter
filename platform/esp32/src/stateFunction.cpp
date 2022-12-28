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