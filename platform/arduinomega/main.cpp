/*
Programme principal de l'arduinomega
*/

#include <TimerOne.h>
#include <Stepper.h>

  Timer1.initialize(1000);  // initialiser le timer avec une période de 1 seconde
  Timer1.attachInterrupt(get_UART_com());  // attacher la fonction maMethode à l'interruption du timer


void setup(){

  Serial.begin(115200);// Initialiser le tact rate pour la communication UART 
}
void loop(){
  get_UART_com();
  if (Serial.available() > 0){
    String start = Serial.readString();
    if(start = = "return_home"){
      return_home();
    }
  }

}


/*-----------------------------------------------------------Methode*------------------------------------------------*/

/*stepper_Stop est une Methode qui lit les communication UART venant de l'Arduino Mega. En fonction des messages communiqués, 
elle vérouille ou déverouille les stepper via le pin 8 (enpin). état HIGH conrrespond à l'activation des stepepr, l'état LOW à la désactivation*/
void stepper_Stop(){

if (Serial.available() > 0) {
    // Lecture du string envoyé par l'ESP
    String message = Serial.readString();
     if (message == "stop") {
    // mettre en route les steppers
    enpin = HIGH; //pin 8 is LOW == Enabel 
    }
    else if(message == "enable"){
    //arrêter les steppers 
    enpin = LOW; // pin 8 is HIGH == Disabel
    }
  }
}


/*get_UART_com est une Méthode qui envoie une commande "get_state" sur le canal de communication UART pour obtenir l'état de la communication.
 Elle est appelée dans le void setup() toutes les secondes */
void get_UART_com(){
  Serial.println("get_state"); //obtenir la variable état 
}