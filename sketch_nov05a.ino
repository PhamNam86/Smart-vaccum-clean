#define ENA   14          // Enable/speed motors Right        GPIO14(D5)
#define ENB   12          // Enable/speed motors Left         GPIO12(D6)
#define IN_1  15          // L298N in1 motors Right           GPIO15(D8)
#define IN_2  13          // L298N in2 motors Right           GPIO13(D7)
#define IN_3  2           // L298N in3 motors Left            GPIO2(D4)
#define IN_4  0           // L298N in4 motors Left            GPIO0(D3)

#define led  16           // L298N in4 motors Left            GPIO0(D0)

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

String command;             //String to store app command state.
int speedCar = 1023;         // 400 - 1023.
int speed_Coeff = 3;


#include <SoftwareSerial.h>
SoftwareSerial SIM(4, 5);   // RX & TX

// Variable to store text message
String textMessage;

// Create a variable to store Lamp state
String lampState = "HIGH";


const char* ssid = "Huong Nam 2.4GHz";
//const char* password = "0868475996";
ESP8266WebServer server(80);
//int rdata; // received charactors
void setup(){

 Serial.begin(115200);

 pinMode(ENA, OUTPUT);
 pinMode(ENB, OUTPUT);  
 pinMode(IN_1, OUTPUT);
 pinMode(IN_2, OUTPUT);
 pinMode(IN_3, OUTPUT);
 pinMode(IN_4, OUTPUT); 
  
pinMode(led, OUTPUT);
analogWrite(led,LOW);
  
  SIM.begin(115200);
  
  delay(5000);
  Serial.println("SIM start...");

  // AT command to set SIM to SMS mode
  SIM.println("AT"); //Handshaking with SIM
  delay(100);
  SIM.println("AT+CMGF=1"); 
  delay(100);
  // Set module to send SMS data to serial out upon receipt 
  SIM.println("AT+CNMI=2,2,0,0,0");
  delay(100);
  
/*
  SIM.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
  SIM.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  updateSerial();
  SIM.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
  updateSerial();
  SIM.println("AT+CREG?"); //Check whether it has registered in the network
  updateSerial();
*/
// Connecting WiFi

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
 
 // Starting WEB-server 
     server.on ( "/", HTTP_handleRoot );
     server.onNotFound ( HTTP_handleRoot );
     server.begin();    
}

void goAhead(){ 

      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(ENB, speedCar);
  }

void goBack(){ 

      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
  }

void goRight(){ 

      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(ENB, speedCar);
  }

void goLeft(){

      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
  }

void goAheadRight(){
      
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(ENA, speedCar/speed_Coeff);
 
      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(ENB, speedCar);
   }

void goAheadLeft(){
      
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(ENB, speedCar/speed_Coeff);
  }

void goBackRight(){ 

      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar/speed_Coeff);

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
  }

void goBackLeft(){ 

      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar/speed_Coeff);
  }

void stopRobot(){  

      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
 }
void smart(){
  if(Serial.available()){
  String rdata = Serial.readString(); //Read the serial data and store it
  
  delay(10);

  if(rdata.indexOf("ON")>=0)
  {
      goBack();
      
    }
  if(rdata.indexOf("OFF")>=0)
   {
         goAhead();
      }
  
  
  }
  delay(100);
}

void loop(){
    if(SIM.available()>0){
    textMessage = SIM.readString();
    Serial.print(textMessage);    
    delay(10);
  } 
  if(textMessage.indexOf("ON")>=0){
    // Turn on relay and save current state
    
    lampState = "on";
    Serial.println("TURN ON");  
    textMessage = "";   
  }
  if(textMessage.indexOf("OFF")>=0){
    // Turn off relay and save current state
    
    lampState = "off"; 
    Serial.println("TURN OFF");
    textMessage = ""; 
  }
  if(textMessage.indexOf("STATE")>=0){
    String message = "Lamp is " + lampState;
    sendSMS(message);                       // send only 1 message
    Serial.println("Lamp state resquest");
    textMessage = "";
  }

    server.handleClient();
    
      command = server.arg("State");
      if (command == "F") goAhead();
      else if (command == "B") goBack();
      else if (command == "L") goLeft();
      else if (command == "R") goRight();
      else if (command == "I") goAheadRight();
      else if (command == "G") goAheadLeft();
      else if (command == "J") goBackRight();
      else if (command == "H") goBackLeft();
      else if (command == "0") speedCar = 400;
      else if (command == "1") speedCar = 470;
      else if (command == "2") speedCar = 540;
      else if (command == "3") speedCar = 610;
      else if (command == "4") speedCar = 680;
      else if (command == "5") speedCar = 750;
      else if (command == "6") speedCar = 820;
      else if (command == "7") speedCar = 890;
      else if (command == "8") speedCar = 960;
      else if (command == "9") speedCar = 1023;
      else if (command == "S") stopRobot(); 
      else if (command == "A") smart();
   
}

// Function that sends SMS
void sendSMS(String message){
  // AT command to set SIM900 to SMS mode
  SIM.println("AT+CMGF=1"); 
  delay(100);

  // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  SIM.println("AT+CMGS=\"+84868475996\""); 
  delay(100);
  // Send the SMS
  SIM.println(message); 
  delay(100);

  // End AT command with a ^Z, ASCII code 26
  SIM.write(26); 
  delay(100);
  SIM.println();
  // Give module time to send SMS
  delay(5000);  
}

void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    SIM.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(SIM.available()) 
  {
    Serial.write(SIM.read());//Forward what Software Serial received to Serial Port
  }
}

void HTTP_handleRoot(void) {

if( server.hasArg("State") ){
       Serial.println(server.arg("State"));
  }
  server.send ( 200, "text/html", "" );
  delay(1);
}
