/*main :
main programm */

#include <Arduino.h>
#include <Servo.h>     // librairie for Servos 
#include <TimerOne.h>  // Librairie for Timers

#define LED_RED   23 // set the pin and variable name for the Red status LED
#define LED_GREEN 24 // set the pin and variable name for the Green status LED
#define LED_BLUE  25 // set the pin and variable name for the Blue status LED

#define SWITCH_INPUT_Start 26 // definir le pin d'entrée du Switch de démarrage du plotter 
#define SWITCH_INPUT_Program 27 // définir le pin d'entrée du Switch de démarrage du programme 

int previousSwitchStateStart = LOW; // Déclarer une variable pour suivre l'état précédent du Switch de démarrage plotter
int previousSwitchStateProgram = LOW; // Déclarer une variable pour suivre l'état précédent du Switch démarrage programme 
int currentSwitchStateStart; // Déclarer une variable pour suivre l'état actuel du Switch de démarrage plotter
int currentSwitchStateProgram; // Déclarer une variable pour suivre l'état actuel du Switch démarrage programme 

Servo monServo; //Déclaration de l'objet monServo


void setup()
{  
// initialisation du matériel et des paramètres de la carte Arduino

monServo.attach(A9); // connecte le servo sur le pin 
pinMode(SWITCH_INPUT_Start, INPUT);// Initialisez le port en entrée pour l'état du Switch de démarrage plotter
pinMode(SWITCH_INPUT_Program, INPUT);// Initialisez le port en entrée pour l'état du Switch de démarrage programme 

// Initialise les pins de la LED RGB en sortie
pinMode(LED_RED, OUTPUT);
pinMode(LED_GREEN, OUTPUT);
pinMode(LED_BLUE, OUTPUT);

  Timer1.initialize(1000);  // initialiser le timer avec une période de 1 seconde
  Timer1.attachInterrupt(stateFunction);  // attacher la fonction maMethode à l'interruption du timer
  Timer2.initialize(1000);
  Timer2.attachInterrupt(get_Switches_States)

Serial.begin(115200);// Initialiser le tact rate pour la communication UART 
}
void loop(){


  if (currentSwitchStateStart != previousSwitchStateStart || currentSwitchStateProgram != previousSwitchStateProgram) { 
    // Si l'état des Switchs de démarrage plotter ou de démarrage programme ont changé depuis la dernière itération
    if (currentSwitchStateStart == LOW) {  // Si le Switch de démarrage plotter est enfoncé (LOW)
      if(currentSwitchStateProgram == LOW){ // Si le Switch de démarrage programme est enfoncé (LOW)
      enable_Motors(); // déverouiller les moteurs
      startFunction(); // Effectuer la fonction de démarrage 
      }
    }
    else if (currentSwitchStateStart == HIGH || currentSwitchStateProgram == HIGH) {
      // Si le Switch de démarrage plotter est désenfoncer (HIGH) ou que le Switch de démarrage programme est désenfoncé (HIGH)
      disable_Motors(); //vérouiller les moteurs 
      stopFunction(); // Effectuer la fonction de démarrage 

    }
  }



}

/*-----------------------------------------------------------Methode*------------------------------------------------/

/*get_Switches_States : est une methode appellée toutes secondes grace à Timer2 pour récuperer l'état des Switchs de démarrage programme et plotter */

void get_Switches_States(){
  currentSwitchStateStart = digitalRead(SWITCH_INPUT_Start); // Lire l'état du Switch de démarrage du plotter
  currentSwitchStateProgram = digitalRead(SWITCH_INPUT_Program); // Lire l'état du Switch de démarrage programme 
  previousSwitchStateStart = currentSwitchStateStart;// Mettez à jour l'état précédent de l'interrupteur
  previousSwitchStateProgram = currentSwitchStateProgram;// Mettez à jour l'état précédent de l'interrupteur
}


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
    String message = Serial.write();

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