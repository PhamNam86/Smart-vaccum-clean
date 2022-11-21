#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>


// Include Software Serial library to communicate with GSM
#include <SoftwareSerial.h>

// Configure software serial port
SoftwareSerial SIM900(4, 5); //RX&TX

const char* ssid = "Node_MCU CAR";
//const char* host = "NodeMCU";

ESP8266WebServer server(80);

// Variable to store text message
String textMessage;

// Create a variable to store Lamp state
String lampState = "HIGH";

// Relay connected to pin 12
const int relay = 12;
int measurePin = A0; //Connect dust sensor to Arduino A0 pin
int ledPower = 2;   //Connect 3 led driver pins of dust sensor to Arduino D2

int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;

float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;
void setup(){
  
  Serial.begin(115200);
  //Begin serial communication with Arduino and SIM800L
   // Set relay as OUTPUT
  pinMode(relay, OUTPUT);

  // By default the relay is off
  digitalWrite(relay, HIGH);
  
  // Initializing serial commmunication
  SIM900.begin(115200);
  pinMode(ledPower,OUTPUT);
  // Give time to your GSM shield log on to network
  delay(5000);
  Serial.println("SIM800L start...");

  // Connecting WiFi

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);
  // Starting WEB-server
    
     server.on ( "/", HTTP_handleRoot );
     server.onNotFound ( HTTP_handleRoot );
     server.begin();    
     
  // AT command to set SIM900 to SMS mode
  SIM900.println("AT"); //Handshaking with SIM900
  delay(100);
  SIM900.println("AT+CMGF=1"); 
  delay(100);
  // Set module to send SMS data to serial out upon receipt 
  SIM900.println("AT+CNMI=2,2,0,0,0");
  delay(100);
}
void smart()
{

digitalWrite(ledPower,LOW); // power on the LED
  delayMicroseconds(samplingTime);

  voMeasured = analogRead(measurePin); // read the dust value

  delayMicroseconds(deltaTime);
  digitalWrite(ledPower,HIGH); // turn the LED off
  delayMicroseconds(sleepTime);

  // 0 - 5V mapped to 0 - 1023 integer values
  // recover voltage
  calcVoltage = voMeasured * (5.0 / 1024.0);

  // linear eqaution taken from http://www.howmuchsnow.com/arduino/airquality/
  // Chris Nafis (c) 2012
  dustDensity = 170 * calcVoltage - 0.1;


  Serial.println("Raw Signal Value (0-1023):");
  Serial.println(voMeasured);

  Serial.println("Voltage:");
  Serial.println(calcVoltage);

  Serial.println("Dust Density:");
  Serial.println(dustDensity); // unit: (muy) ug/m3

  if(dustDensity > 210)
  {
    Serial.println("HIGH");
    }
  else{
    Serial.println("LOW");
    }
  
  delay(2000);

}
void loop(){
  if(SIM900.available()>0){
    textMessage = SIM900.readString();
    Serial.print(textMessage);    
    delay(10);
  } 
  if(textMessage.indexOf("STOP")>=0){
    // Turn on relay and save current state
    //digitalWrite(relay, LOW);
    lampState = "stop";
    Serial.println("S");  
    textMessage = "";   
  }
  if(textMessage.indexOf("FORWARD")>=0){
    // Turn off relay and save current state
    digitalWrite(relay, HIGH);
    lampState = "go ahead"; 
    Serial.println("F");
    textMessage = ""; 
  }
  if(textMessage.indexOf("BACKWARD")>=0){
    String message = "Lamp is " + lampState;
    sendSMS(message);
    Serial.println("B");
    textMessage = "";
  }
  


  server.handleClient();
  
  String command = server.arg("State");
    if (command == "A") smart();
  delay(10);
}

// Function that sends SMS
void sendSMS(String message){
  // AT command to set SIM900 to SMS mode
  SIM900.println("AT+CMGF=1"); 
  delay(100);

  // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  SIM900.println("AT+CMGS=\"+84868475996\""); //cmgsex
  delay(100);
  // Send the SMS
  SIM900.println(message); 
  delay(100);

  // End AT command with a ^Z, ASCII code 26
  SIM900.write(26); 
  delay(100);
  SIM900.println();
  // Give module time to send SMS
  delay(5000);  
}

void HTTP_handleRoot(void) 
{
if( server.hasArg("State") ){
      Serial.println(server.arg("State"));
  }
  server.send (200,"text/html","" );
  delay(1);
}

