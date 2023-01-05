/*
 * Blink on arduino
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */


#include <Arduino.h>
#include <SoftwareSerial.h>
#define UART_ESP_TX 17
#define UART_ESP_RX 16
#define TEST_LED 21
const char uartEndSymbol='~';

SoftwareSerial uartArduino(UART_ESP_RX, UART_ESP_TX);


void writeCommandArduino(String command){
    uartArduino.print(command);
    //Signaling the end of the Message
    uartArduino.print(uartEndSymbol);
    //waiting for the message to be sent
    uartArduino.flush();
}
String readArduino(){
    if (uartArduino.available() > 0) {
        String message=uartArduino.readStringUntil(uartEndSymbol);
        return message;
        //Debug
        //Serial.println(message);
    }
    else{
        return "";
    }

}
void testUart(){
    //Test Uart
    writeCommandArduino("on");
    digitalWrite(TEST_LED,HIGH);
    delay(1000);
    writeCommandArduino("off");
    digitalWrite(TEST_LED,LOW);
    delay(1000);
}

void setup()
{
    Serial.begin(9600);
    uartArduino.begin(9600);
    pinMode(TEST_LED, OUTPUT);


}
void loop() {
    testUart();
}