/*main :
main programm */

#include <Arduino.h>
#include <Servo.h>     // Librairie for Servos
//#include <TimerOne.h>  // Librairie for Timers
#include <SoftwareSerial.h>
#include <WiFi.h>
//#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
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
bool ledState = 0;
int previousSwitchStateStart = LOW; // Declare a variable to track the previous state of the plotter start switch
int previousSwitchStateProgram = LOW; // Declare a variable to track the previous state of the program start switch
int currentSwitchStateStart; // Declare a variable to track the actual state of the plotter start switch
int currentSwitchStateProgram; // Declare a variable to track the actual state of the programm start switch
double servo_angle = 90; // Declare a variable for the setting of the servos angle

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
  html {
    font-family: Arial, Helvetica, sans-serif;
    text-align: center;
  }
  h1 {
    font-size: 1.8rem;
    color: white;
  }
  h2{
    font-size: 1.5rem;
    font-weight: bold;
    color: #143642;
  }
  .topnav {
    overflow: hidden;
    background-color: #143642;
  }
  body {
    margin: 0;
  }
  .content {
    padding: 30px;
    max-width: 600px;
    margin: 0 auto;
  }
  .card {
    background-color: #F8F7F9;;
    box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5);
    padding-top:10px;
    padding-bottom:20px;
  }
  .button {
    padding: 15px 50px;
    font-size: 24px;
    text-align: center;
    outline: none;
    color: #fff;
    background-color: #0f8b8d;
    border: none;
    border-radius: 5px;
    -webkit-touch-callout: none;
    -webkit-user-select: none;
    -khtml-user-select: none;
    -moz-user-select: none;
    -ms-user-select: none;
    user-select: none;
    -webkit-tap-highlight-color: rgba(0,0,0,0);
   }
   /*.button:hover {background-color: #0f8b8d}*/
   .button:active {
     background-color: #0f8b8d;
     box-shadow: 2 2px #CDCDCD;
     transform: translateY(2px);
   }
   .state {
     font-size: 1.5rem;
     color:#8c8c8c;
     font-weight: bold;
   }
  </style>
<title>ESP Web Server</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" href="data:,">
</head>
<body>
  <div class="topnav">
    <h1>ESP WebSocket Server</h1>
  </div>
  <div class="content">
    <div class="card">
      <h2>Output - GPIO 2</h2>
      <p class="state">state: <span id="state">%STATE%</span></p>
      <p><button id="button" class="button">Toggle</button></p>
    </div>
  </div>
<script>
  var gateway = `ws://${window.location.hostname}/ws`;
  var websocket;
  window.addEventListener('load', onLoad);
  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; // <-- add this line
  }
  function onOpen(event) {
    console.log('Connection opened');
  }
  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }
  function onMessage(event) {
    var state;
    if (event.data == "1"){
      state = "ON";
    }
    else{
      state = "OFF";
    }
    document.getElementById('state').innerHTML = state;
  }
  function onLoad(event) {
    initWebSocket();
    initButton();
  }
  function initButton() {
    document.getElementById('button').addEventListener('click', toggle);
  }
  function toggle(){
    websocket.send('toggle');
  }
</script>
</body>
</html>
)rawliteral";

Servo monServo; // Declaration of the servomotor mounted in the pen mechanism
SoftwareSerial uartArduino(UART_ESP_RX, UART_ESP_TX);
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");


/*-----------------------------------------------------------Methode*------------------------------------------------*/

void notifyClients() {
    ws.textAll(String(ledState));
}
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        data[len] = 0;
        if (strcmp((char*)data, "toggle") == 0) {
            ledState = !ledState;
            notifyClients();
        }
    }
}
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
    switch (type) {
        case WS_EVT_CONNECT:
            Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
            break;
        case WS_EVT_DISCONNECT:
            Serial.printf("WebSocket client #%u disconnected\n", client->id());
            break;
        case WS_EVT_DATA:
            handleWebSocketMessage(arg, data, len);
            break;
        case WS_EVT_PONG:
        case WS_EVT_ERROR:
            break;
    }
}
void initWebSocket() {
    ws.onEvent(onEvent);
    server.addHandler(&ws);
}
String processor(const String& var){
    Serial.println(var);
    if(var == "STATE"){
        if (ledState){
            return "ON";
        }
        else{
            return "OFF";
        }
    }
}
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

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }

    // Print ESP Local IP Address
    Serial.println(WiFi.localIP());

    initWebSocket();

    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_html, processor);
    });

    // Start server
    server.begin();
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
    ws.cleanupClients();
    digitalWrite(TEST_LED, ledState);

}