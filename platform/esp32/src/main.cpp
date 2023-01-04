/*main :
programme principal */

#include <Arduino.h>
#include <Servo.h>     // bibliothèque pour utiliser les Servos 
#include <TimerOne.h>  // bibliothèque pour utiliser les timers

#define LED_RED   9 //R
#define LED_GREEN 10 //G
#define LED_BLUE  11 //B

#define SWITCH_INPUT 2 // definir le pin d'entrée du Switch 


int previousSwitchState = LOW; // Déclarez une variable pour suivre l'état précédent de l'interrupteur

Servo monServo; //Déclaration de l'objet monServo


void setup()
{  
// initialisation du matériel et des paramètres de la carte Arduino

monServo.attach(A9); // connecte le servo sur le pin 
pinMode(SWITCH_INPUT, INPUT);// Initialisez le port en entrée pour l'état du switch

// Initialise les pins de la LED RGB en sortie
pinMode(LED_RED, OUTPUT);
pinMode(LED_GREEN, OUTPUT);
pinMode(LED_BLUE, OUTPUT);

  Timer1.initialize(1000);  // initialiser le timer avec une période de 1 seconde
  Timer1.attachInterrupt(stateFunction);  // attacher la fonction maMethode à l'interruption du timer

Serial.begin(115200);// Initialiser le tact rate pour la communication UART 
}
void loop(){

  int currentSwitchState = digitalRead(SWITCH_INPUT); // Lire l'état de l'interrupteur

  if (currentSwitchState != previousSwitchState) { // Si l'état de l'interrupteur a changé depuis la dernière itération
    if (currentSwitchState == LOW) {  // Si l'interrupteur est enfoncé (LOW), démarrez la fonction
      startFunction();
      enable_Motors();
    }
    else if (currentSwitchState == HIGH) {// Si l'interrupteur est relâché (HIGH), arrêtez la fonction
      stopFunction();
      disable_Motors();
    }
  }

  previousSwitchState = currentSwitchState;// Mettez à jour l'état précédent de l'interrupteur


}

/*-----------------------------------------------------------Methode*------------------------------------------------/


/*startFunction : initialise toutes les fonctions de wallplotter et verifie l'état de celui-ci. Elle appelle donc tout les fonctions de type moteur ou sensor 
 l'état du robot sera visible via la LED RGB */

void startFunction() {
  // Insérez ici le code à exécuter lorsque la fonction est démarrée
  UART_return_home();
  servoOff();

}


/*servoOn : met en place le stylo en tournant le servo */
void servoOn() {
monServo.write(90); // fait tourner le servo de 90 degrés
delay(1000); // attend 1 seconde avant de continuer
}


/* servoOff : retracte le stylo en tournant le servo */

void servoOff() {
monServo.write(-90); // fait tourner le servo de 90 degrés
delay(1000); // attend 1 seconde avant de continuer
}


/* stateFunction : vérifie l'état du robot. Elle est appellée toute les secondes dans la fonction principale pour afficer un état en continu */

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


/* */

void disable_Motors() {
 // Envoi du message d'arrêt des moteurs pas à pas à l'Arduino Mega
  Serial.write("stop\n");
  }


/**/

void enable_Motors() {
  // Envoi du message d'activation des moteurs pas à pas à l'Arduino Mega
  Serial.write("enable\n");
  }


/* stopFunction : éteint tout les moteurs */

void stopFunction() {
//Insérer ici le code à executer pour arreter le robot 
servoOff();
disable_Motors();
}


/* envoie un message via UART à l'Adruino Mega pour metre le robot en position initiale. Le message "return_home" activera 
la methode du même nom dans le main de l'Arduino Mega */

void UART_return_home{
  Serial.write("return_home\n");
}