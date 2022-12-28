class StepperController {
  public:
    StepperController() {

    Serial.begin(9600);// Initialiser le tact rate pour la communication UART
    }

    void stopMotors() {
      // Envoi du message d'arrêt des moteurs pas à pas à l'Arduino Mega
      Serial.write("stop\n");
    }
    void enableMotors() {
      // Envoi du message d'activation des moteurs pas à pas à l'Arduino Mega
      Serial.write("enable\n");
    }
};
