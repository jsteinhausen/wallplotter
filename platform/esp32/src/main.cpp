/*
 * Blink on arduino
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#include <SoftwareSerial.h>
#include <Arduino.h>

#define UART_ESP_TX 17
#define UART_ESP_RX 16


SoftwareSerial uart_esp(UART_ESP_RX, UART_ESP_TX);


void setup()
{
    uart_esp.begin(9600);

}
void loop() {

    //Test Uart
    uart_esp.write("On\n");
    delay(5000000);
    uart_esp.write("Off\n");

}