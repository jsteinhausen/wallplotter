
// Created by Johannes Steinhausen on 31.12.22.
//
#include <Arduino.h>
#include <SoftwareSerial.h>

#define UART_ESP_TX 1
#define UART_ESP_RX 0


SoftwareSerial uart_esp(UART_ESP_RX, UART_ESP_TX);


void setup()
{
    //Initialize Build In LED
    pinMode(LED_BUILTIN, OUTPUT);
    //Initialize Uart interface
    uart_esp.begin(9600);
    digitalWrite(LED_BUILTIN,LOW);
}
void loop() {

    //Test Uart
    char c = uart_esp.read();
    if (c=="On"){
        digitalWrite(LED_BUILTIN,HIGH)
    }
    else {
        digitalWrite(LED_BUILTIN, LOW);
    }


}
