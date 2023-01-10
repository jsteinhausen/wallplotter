/*main :
main programm */

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <WiFi.h>

#define UART_ESP_TX 17
#define UART_ESP_RX 16
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
int previousMillis;
int commandLength=7;
double servo_angle = 90; // Declare a variable for the setting of the servos angle
char* myStrings[]={"Eins", "Zwei", "",
                     "", "", "", ""};

char* testCommands[]={"move_pen_abs(12147.04,7806.9);", "draw_line(0,0,0,-3908.4);", "draw_line(0,0,518.57,0);", "draw_line(0,0,0,1610.3);", "draw_line(0,0,2019.7,0);", "draw_line(0,0,0,-1610.3);}","endfile();"};
SoftwareSerial uartArduino(UART_ESP_RX, UART_ESP_TX);
WiFiServer server(8088);

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

    Serial.begin(9600);
// Initialize the tact rate for UART communication
    uartArduino.begin(9600);


    digitalWrite(LED_RED, LOW);

    previousMillis = millis();

// Setting Up Access Point
    Serial.println("Setting AP (Access Point)…");
// Remove the password parameter, if you want the AP (Access Point) to be open , password
/*WiFi.softAP(ssidAP);
IPAddress IP = WiFi.softAPIP();
Serial.print("AP IP address: ");
Serial.println(IP);*/


// Start server
/*server.begin();
    // Start server
    server.begin();
    // listen for client



    WiFiClient client;
    //Waits for client(pc) to connect
    int counter=0;
    while (!client) {
        client = server.available();
        if(counter==1000) {
            Serial.println("Waiting for client....");
            counter=0;
        }
        counter++;
        delay(1);
    }
    // check client is connected
    Serial.println("Client is connected");
    int testArray[]={0,1,2,3};
    int myStringCounter=0;
    int innerCounter=0;
    //unsigned int
    const int BUFFER_SIZE=30;
    char data[BUFFER_SIZE];
    char* receivingString[BUFFER_SIZE];
    while (client.connected()) {
        if (client.available()) {
            int len= client.readBytesUntil(uartEndSymbol,data,BUFFER_SIZE-1);
            if(len < BUFFER_SIZE){
                //Add NULL at the end of each data byte
                data[len] = '\0';
            }else {
                data[BUFFER_SIZE] = '\0';
            }
            //char vector
            char* charVTemp;
            charVTemp=data;

            receivingString[myStringCounter]= charVTemp;
            Serial.print("MyStringCounter: ");
            Serial.println(myStringCounter);
            Serial.print("receivingString: ");
            Serial.println(receivingString[myStringCounter]);
            testArray[myStringCounter]=myStringCounter;
            Serial.print("TestArray: ");
            Serial.println(testArray[myStringCounter]);
            myStringCounter++;
        }
        Serial.println(innerCounter);
        innerCounter++;
        //delay(1);
    }
    Serial.println("All receiving Strings: ");
    for(int i=0;i<BUFFER_SIZE;i++){
        //myStrings[i]=receivingString[i];
        Serial.println(receivingString[i]);
    }
    Serial.println("Client is disconnected");
    Serial.print("TestArray: ");
    Serial.println(testArray[0]);*/
}

/*-----------------------------------------------------------Methode*------------------------------------------------*/
void debugPrintln(String string){
    Serial.println(string);
}

/*get_Switches_States : is a method called every second by Timer2 to get the state of the program and plotter start switches */

void get_Switches_States(){

  //noInterrupts(); // Block the interrupt to allow the copy in the variables
  currentSwitchStateStart = digitalRead(SWITCH_INPUT_Start); // read the state of the plotter start switch
  //currentSwitchStateProgram = digitalRead(SWITCH_INPUT_Program); //read the state of the program start switch
  //interrupts(); // restart interrupts
  previousSwitchStateStart = currentSwitchStateStart;// Update the previous state variable of the plotter start switch
  previousSwitchStateProgram = currentSwitchStateProgram;// Update the previous state variable of the program start switch
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
    //debugPrintln(command);
}

String readArduino(){
    int counter=0;
    while (uartArduino.available() == 0) {
        if(counter==1000){
            debugPrintln("No data in Stream");
            counter=0;
        }
        //delay(1);
        counter++;
    }
    String message="";
    message=uartArduino.readStringUntil(uartEndSymbol);
    debugPrintln(message);
    return message;
}

void disable_Motors() {
 Serial.write("stop\n"); // Sending the stepper motors stop message to the Arduino Mega
  }

void testUart(){
    //Test Uart
    writeCommandArduino("on");
    digitalWrite(LED_RED,HIGH);
    delay(1000);
    writeCommandArduino("off");
    digitalWrite(LED_RED,LOW);
    delay(1000);
}
/*enable_Motors sends a message to the Arduino Mega via the UART communication protocol to unlock the steppers */

void enable_Motors() {
  Serial.write("enable\n"); // Sending the stepper motors unlock message to the Arduino Mega
}


/* stop_Method : turns off all motors */

void stop_Method() {

  //Insert here the code to execute to stop the robot
  disable_Motors();
  currentSwitchStateProgram == HIGH;
}


/* Return_Home_UART sends a message via the UART communication protocol to the Adruino Mega to put the robot in the home position. The message "return_home" will activate
the method of the same name in the main of the Arduino Mega */

void Return_Home_UART(){

  Serial.write("return_home\n"); // Sending the return_home message to the Arduino Mega
}


/*Start_Method : initializes all the wallplotter functions and checks the state of the wallplotter. It calls all the functions of type motor or sensor
 the state of the robot will be visible via the RGB LED */

void Start_Method() {
    // Insert here the code to execute when the method is started
    Return_Home_UART();
}
void functionToExecuteEverySecond() {

    // Code à exécuter toutes les secondes
    get_Switches_States();
    State_Method();
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
    //testUart();
    //Handling the commands for the arduino
    bool confirmed=0;
    //for(int i;i<commandLength;i++){
        writeCommandArduino(testCommands[0]);
        debugPrintln(testCommands[0]);
        while(!confirmed){
            String confirm="";
            confirm=readArduino();
            if(confirm=="confirm") confirmed=1;
            debugPrintln(confirm);
        }

    //}

}