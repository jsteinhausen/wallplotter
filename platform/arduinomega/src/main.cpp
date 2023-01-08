
// Created by Johannes Steinhausen on 31.12.22.
//
#include <Arduino.h>
#include <SoftwareSerial.h>

#define UART_ESP_TX 14
#define UART_ESP_RX 15
int state=0;
const char uartEndSymbol='~';
SoftwareSerial uartEsp(UART_ESP_RX, UART_ESP_TX);

String readCommandEsp(){
    if (uartEsp.available() > 0) {
        String message=uartEsp.readStringUntil(uartEndSymbol);
        return message;
        //Debug
        //Serial.println(message);
    }
    else{
        return "";
    }
}
void writeEsp(String message){
    uartEsp.print(message);
    //Signaling the end of the Message
    uartEsp.print(uartEndSymbol);
    //waiting for the message to be sent
    uartEsp.flush();
}
void testUart(){
    //Test Uart
    String message=readCommandEsp();

    //Testing state
    if (message == "on") {
            state = 1;
            digitalWrite(LED_BUILTIN, HIGH);
    } else if (message == "off") {
            state = 0;
            digitalWrite(LED_BUILTIN, LOW);
    }


    Serial.println(state);
    switch(state){
        case 0:
            digitalWrite(LED_BUILTIN, LOW);
            break;
        case 1:
            digitalWrite(LED_BUILTIN,HIGH);
            break;
        default:
            digitalWrite(LED_BUILTIN,LOW);
    }
}

void setup()
{
    Serial.begin(9600);
    //Initialize Build In LED
    pinMode(LED_BUILTIN, OUTPUT);
    //Initialize Uart interface
    uartEsp.begin(9600);
    digitalWrite(LED_BUILTIN,HIGH);
}
void loop() {
    testUart();
}
