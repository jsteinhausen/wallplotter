/*
servoOff
retracte le stylo en tournant le servo 
*/

void servoOff() {
monServo.write(-90); // fait tourner le servo de 90 degrés
delay(1000); // attend 1 seconde avant de continuer
}