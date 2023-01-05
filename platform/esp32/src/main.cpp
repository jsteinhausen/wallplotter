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


SoftwareSerial uart_esp(UART_ESP_RX, UART_ESP_TX);


void setup()
{
    uart_esp.begin(9600);
    pinMode(TEST_LED, OUTPUT);


}
void loop() {

    //Test Uart
    uart_esp.print("on;");
    uart_esp.flush();
    digitalWrite(TEST_LED,HIGH);
    delay(1000);
    uart_esp.print("off;");
    digitalWrite(TEST_LED,LOW);
    uart_esp.flush();
    delay(1000);
}