
// Created by Johannes Steinhausen on 31.12.22.
//
#include <Arduino.h>
#include <SoftwareSerial.h>

#define UART_ESP_TX 14
#define UART_ESP_RX 50
int state=0;
const char uartEndSymbol='~';
const String CONFIRM="confirmed";
SoftwareSerial uartEsp(UART_ESP_RX, UART_ESP_TX);

void debugPrintln(String string){
    Serial.println(string);
}

String readCommandEsp(){
    int counter=0;
    String message="";
    while (uartEsp.available() == 0) {
        if(counter==1000){
            debugPrintln("No data in Stream");
            counter=0;
        }
        delay(1);
        counter++;
    }
    message=uartEsp.readStringUntil(uartEndSymbol);
    debugPrintln(message);
    return message;
}
void writeEsp(String message){
    uartEsp.print(message);
    //Signaling the end of the Message
    uartEsp.print(uartEndSymbol);
    debugPrintln(message);
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
    //testUart();
    String command="";
    command=readCommandEsp();
    debugPrintln(command);
    writeEsp(CONFIRM);
    //debugPrintln(CONFIRM);
    delay(1000);
}
