    void stopMotors() {
      // Envoi du message d'arrêt des moteurs pas à pas à l'Arduino Mega
      Serial.write("stop\n");
    }
    void enableMotors() {
      // Envoi du message d'activation des moteurs pas à pas à l'Arduino Mega
      Serial.write("enable\n");
    }