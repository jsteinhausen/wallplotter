/*main :
main programm */

#include <Arduino.h>
#include <Servo.h>     // Librairie for Servos
//#include <TimerOne.h>  // Librairie for Timers
#include <SoftwareSerial.h>
#include <WiFi.h>
//#include <AsyncTCP.h>
//#include <ESPAsyncWebServer.h>
#define UART_ESP_TX 17
#define UART_ESP_RX 16
#define TEST_LED 21
#define LED_RED   23 // Set the pin and variable name for the Red status LED
#define LED_GREEN 24 // Set the pin and variable name for the Green status LED
#define LED_BLUE  25 // Set the pin and variable name for the Blue status LED

#define SWITCH_INPUT_Start 26 // Set the input pin of the plotter start switch
#define SWITCH_INPUT_Program 27 // Set the input pin of the program start switch
const char uartEndSymbol='~';
const char* ssid = "Otterbach 13";
const char* password = "61621584259259194615";
const char* ssidAP = "wallplotter";
const char* passwordAP = "trinat2020";
bool ledState = 0;
int previousSwitchStateStart = LOW; // Declare a variable to track the previous state of the plotter start switch
int previousSwitchStateProgram = LOW; // Declare a variable to track the previous state of the program start switch
int currentSwitchStateStart; // Declare a variable to track the actual state of the plotter start switch
int currentSwitchStateProgram; // Declare a variable to track the actual state of the programm start switch
int clientCounter=0;
double servo_angle = 90; // Declare a variable for the setting of the servos angle
/*char *myStrings[] = {"", "", "",
                     "", "", "", ""
};*/
Servo monServo; // Declaration of the servomotor mounted in the pen mechanism
SoftwareSerial uartArduino(UART_ESP_RX, UART_ESP_TX);
WiFiServer server(8088);



/*-----------------------------------------------------------Methode*------------------------------------------------*/


/*get_Switches_States : is a method called every second by Timer2 to get the state of the program and plotter start switches */

void get_Switches_States(){

  currentSwitchStateStart = digitalRead(SWITCH_INPUT_Start); // read the state of the plotter start switch
  currentSwitchStateProgram = digitalRead(SWITCH_INPUT_Program); //read the state of the program start switch
  previousSwitchStateStart = currentSwitchStateStart;// Update the previous state variable of the plotter start switch
  previousSwitchStateProgram = currentSwitchStateProgram;// Update the previous state variable of the program start switch
}


/*Start_Method : initializes all the wallplotter functions and checks the state of the wallplotter. It calls all the functions of type motor or sensor
 the state of the robot will be visible via the RGB LED */



/*Servo_On: sets up the pen by turning the servo */

void Servo_On() {
monServo.write(servo_angle); // rotates the servo 90 degrees
delay(500); // wait half a second before continuing
}


/* Servo_Off: retracts the pen by turning the servo */

void Servo_Off() {

monServo.write(-servo_angle); // rotates the servo - 90 degrees
delay(500); // wait half a second before continuing
}


/*State_Methodcks the state of the robot. It is called every second
by Timer1 to display a continuous state of the plotter thanks to the RGB LED */

void State_Method(){

  // Send a request to get the state variable
  Serial.println("get_state");
  // If a message is available on the UART line, read it
  if (Serial.available()) {
    String message = Serial.readString();
    if (message == "stop") { //if string message is stop
      // Motor stopped: lights up the red LED
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_BLUE, LOW);
    }
    else if (message == "enable") { // if string message is enable
      // Motors are moving : lights up the green LED
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_BLUE, LOW);
    }
    else if (Serial.available() > 0) { // ESP32 sends a message via UART
      // lights up the blue LED
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_BLUE, HIGH);
    }
  }
}


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

void disable_Motors() {
 Serial.write("stop\n"); // Sending the stepper motors stop message to the Arduino Mega
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
/*enable_Motors sends a message to the Arduino Mega via the UART communication protocol to unlock the steppers */

void enable_Motors() {
  Serial.write("enable\n"); // Sending the stepper motors unlock message to the Arduino Mega
}


/* stop_Method : turns off all motors */

void stop_Method() {

  //Insert here the code to execute to stop the robot
  Servo_Off();
  disable_Motors();
  currentSwitchStateProgram == HIGH;
}


/* Return_Home_UART sends a message via the UART communication protocol to the Adruino Mega to put the robot in the home position. The message "return_home" will activate
the method of the same name in the main of the Arduino Mega */

void Return_Home_UART(){

  Serial.write("return_home\n"); // Sending the return_home message to the Arduino Mega
}
void Start_Method() {
    // Insert here the code to execute when the method is started
    Return_Home_UART();
    Servo_Off();
}

void setup(){

// initialization of the hardware and the parameters of the ESP32 board
//monServo.attach(A9); // connect the servo to the analog pin number 9
pinMode(SWITCH_INPUT_Start, INPUT);// Initialize the input port for the plotter start switch state
pinMode(SWITCH_INPUT_Program, INPUT);// Initialize the input port for the program start switch state

// Initialize the pins of the output RGB LED
pinMode(LED_RED, OUTPUT);
pinMode(LED_GREEN, OUTPUT);
pinMode(LED_BLUE, OUTPUT);

//Timer1.initialize(1000);  // initialize timer 1 and 2 with a period of 1 second
//Timer2.initialize(1000);
//Timer1.attachInterrupt(State_Method); // attach State_Method et get_Switches_States to the respective interrupt of timers 1 and 2
//Timer2.attachInterrupt(get_Switches_States);

Serial.begin(115200); // Initialize the tact rate for UART communication
uartArduino.begin(9600);
pinMode(TEST_LED, OUTPUT);

    digitalWrite(TEST_LED, LOW);

    // Setting Up Access Point
    Serial.print("Setting AP (Access Point)…");
    // Remove the password parameter, if you want the AP (Access Point) to be open , password
    WiFi.softAP(ssidAP);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
    /*while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }

    // Print ESP Local IP Address
    Serial.println(WiFi.localIP());*/


    // Start server
    server.begin();
    /* listen for client */

    uint8_t data[30];
    int counter=0;

    WiFiClient client;
    //Waits for client(pc) to connect
    while (!client) {
        client = server.available();
        if(counter==1000) {
            Serial.println("Waiting for client....");
            counter=0;
        }
        counter++;
        delay(1);
    }
    /* check client is connected */
    Serial.println("Client is connected");
    while (client.connected()) {
        //for(int i=0;i<6;i++){
        if (client.available()) {
            int len = client.read(data, 30);
            if(len < 30){
                data[len] = '\0';
            }else {
                data[30] = '\0';
            }
            //char* charVTemp;
            //charVTemp=(char*)data;
            /*for(int j=0;j<=len;j++){
               charVTemp[j]=data[j];
            }*/
            //myStrings[0]=charVTemp;
        //}
        }
    }
    /*for (int i = 0; i < 6; i++) {
        Serial.println(myStrings[i]);
        delay(500);
    }*/


}







void loop(){
/*
  if (currentSwitchStateStart != previousSwitchStateStart || currentSwitchStateProgram != previousSwitchStateProgram) {
    // If the state of the plotter start or program start switches have changed since the last iteration.
    if (currentSwitchStateStart == LOW) {  // If the plotter start switch is pressed (LOW)
      if(currentSwitchStateProgram == LOW){ // If the program start switch is pressed (LOW)
      enable_Motors(); // unlock the motors
      Start_Method(); // Execute the start-up method
      }
    }
    else if (currentSwitchStateStart == HIGH || currentSwitchStateProgram == HIGH) {
      // If the plotter start switch is down (HIGH) or the program start switch is down (HIGH)
      disable_Motors(); //lock the motors
      stop_Method(); // Execute the stop method

    }
  }*/
    Serial.println("Process finished");
    delay(1000);


}