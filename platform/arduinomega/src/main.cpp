
// Created by Johannes Steinhausen on 31.12.22.
//
#include <Arduino.h>
#include <SoftwareSerial.h>

#define UART_ESP_TX 14
#define UART_ESP_RX 50
#define TEST_LED 48
int state=0;
const char uartEndSymbol='~';
SoftwareSerial uartEsp(UART_ESP_RX, UART_ESP_TX);

String readCommandEsp(){
    uartEsp.listen();
    if (uartEsp.available() > 0) {
        String message=uartEsp.readStringUntil(uartEndSymbol);
        //Debug
        Serial.println(message);
        return message;

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
            digitalWrite(TEST_LED, HIGH);
    } else if (message == "off") {
            state = 0;
            digitalWrite(TEST_LED, LOW);
    }
    else state=2;


    Serial.println(state);
    switch(state){
        case 0:
            digitalWrite(TEST_LED, LOW);
            break;
        case 1:
            digitalWrite(TEST_LED,HIGH);
            break;
        default:
            digitalWrite(TEST_LED,LOW);
    }
}

void setup()
{
    Serial.begin(9600);
    //Initialize Build In LED
    pinMode(TEST_LED, OUTPUT);
    //Initialize Uart interface
    uartEsp.begin(9600);
    digitalWrite(TEST_LED,HIGH);
}
void loop() {
    testUart();
}
