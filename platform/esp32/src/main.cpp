/*
 * Blink on arduino
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */


#include <Arduino.h>
#include <SoftwareSerial.h>
#define UART_ESP_TX 17
#define UART_ESP_RX 16
#define TEST_LED 2


SoftwareSerial uart_esp(UART_ESP_RX, UART_ESP_TX);


void setup()
{
    uart_esp.begin(9600);
    pinMode(TEST_LED, OUTPUT);


}
void loop() {

    //Test Uart
    uart_esp.write("1");
    digitalWrite(TEST_LED,HIGH);
    delay(5000000);
    uart_esp.write("0");
    digitalWrite(TEST_LED,LOW);

}