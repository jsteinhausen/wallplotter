/*
 * stateFunction
 * vérifie l'état du robot :
 * Elle est appellée toute les secondes dans la fonction principale pour afficer un état en continu 
 */
void stateFunction(){
    // Envoie une requête pour obtenir la variable état
  Serial.println("get_state");

  // Si un message est disponible sur la ligne UART, lit-le
  if (Serial.available()) {
    String message = stepperController.Serial.write();

    if (message == "stop") {
      // Moteur à l'arrêt : allume la LED rouge
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_BLUE, LOW);
    }
    else if (message == "enable") {
      // Moteur en mouvement : allume la LED verte
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_BLUE, LOW);
    }
    else if (Serial.available() > 0) {
      // ESP32 envoie un message sur Serial
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_BLUE, HIGH);
    }
  }
}