
// Created by Johannes Steinhausen on 31.12.22.
//
#include <Arduino.h>
#include <SoftwareSerial.h>

#define UART_ESP_TX 1
#define UART_ESP_RX 0
int state=0;

SoftwareSerial uart_esp(UART_ESP_RX, UART_ESP_TX);

String readCommandEsp(){
    if (uart_esp.available() > 0) {
        return uart_esp.readStringUntil('~');
        //Debug
        //Serial.println(message);
    }
    else{
        return "";
    }
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
    uart_esp.begin(9600);
    digitalWrite(LED_BUILTIN,HIGH);
}
void loop() {
    testUart();
}
