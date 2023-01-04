
// Created by Johannes Steinhausen on 31.12.22.
//
#include <Arduino.h>
#include <SoftwareSerial.h>

#define UART_ESP_TX 1
#define UART_ESP_RX 0
int state=0;

SoftwareSerial uart_esp(UART_ESP_RX, UART_ESP_TX);


void setup()
{
    Serial.begin(9600);
    //Initialize Build In LED
    pinMode(LED_BUILTIN, OUTPUT);
    //Initialize Uart interface
    uart_esp.begin(9600);
    digitalWrite(LED_BUILTIN,LOW);
}
void loop() {

    //Test Uart
    String message = uart_esp.readString();
    //Testing state
    if (state==0){
        if (message=="on"){
            state=1;
        }
        else if (message=="off"){
            state=0;
        }
    }
    else if (state==1){
        if (message=="off"){
            state=0;
        }
        else if(message=="on"){
            state=1;
        }
    }

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
