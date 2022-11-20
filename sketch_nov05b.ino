const int TRIG_PIN = 16; // Arduino pin connected to Ultrasonic Sensor's TRIG pin
const int ECHO_PIN = 17; // Arduino pin connected to Ultrasonic Sensor's ECHO pin
float duration_us, distance_cm;

String cdata; 
String textMessage;
int command;

int led = 5;
 

int measurePin = A0; //Connect dust sensor to Arduino A0 pin
int ledPower = 2;   //Connect 3 led driver pins of dust sensor to Arduino D2

int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;

float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

void setup()
{
 
 Serial.begin(115200); 

 pinMode(TRIG_PIN, OUTPUT); // set arduino pin to output mode
 pinMode(ECHO_PIN, INPUT);  // set arduino pin to input mode

 pinMode(led, OUTPUT);

 pinMode(ledPower,OUTPUT);

 digitalWrite(led, LOW);
}
//text message
void control_speed()
{
   if(Serial.available()>0){
   textMessage = Serial.readString();
    Serial.println(textMessage);
    //updateSerial();    
    delay(10);
  } 
  if(textMessage.indexOf("TURN ON")>=0)
  {
    analogWrite(led, 255);
    }
  if(textMessage.indexOf("MEDIUM")>=0)
    {
      analogWrite(led, 200);
      }
      if(textMessage.indexOf("LOWEST")>=0)
    {
      analogWrite(led, 150);
      }
      if(textMessage.indexOf("TURN OFF")>=0)
    {
      analogWrite(led, 0);
      }
}

void highest()
{
  analogWrite(led, 255);
  }
void medium()
{
  analogWrite(led, 200);
  }
void lowest()
{
  analogWrite(led, 150);
  }
  void stop_mos()
{
  analogWrite(led,0);
  }
void loop()
{
  // code

 while(Serial.available()){ 
  command = Serial.read();
  
      if (command == 'O') highest();
      else if (command == 'M') medium();
      else if (command == 'C') lowest();
      else if (command == 'P') stop_mos();
      //else if (command == "A") smart();
  }
  
  // generate 10-microsecond pulse to TRIG pin
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // measure duration of pulse from ECHO pin
  duration_us = pulseIn(ECHO_PIN, HIGH);
  // calculate the distance
  distance_cm = 0.017 * duration_us;
  
  
  
  // print the value to Serial Monitor
  //Serial.print("distance: ");
  //Serial.print(distance_cm);
  //Serial.println(" cm");
  
  //Serial.println(a);
  delay(500);
  
  int sdata1 = digitalRead(19);        //IR SENSOR
  //Serial.println(sdata1); 
  int sdata2 = round(distance_cm);    //ULTRASOUND SENSOR
  //Serial.println(sdata2); 

  if((sdata1 == 1) || (sdata2 <= 5))
  {

      Serial.println("ON");
     
    }
    else
    {
      Serial.println("OFF");
      }
   //cdata = cdata + sdata1+","+sdata2; // comma will be used a delimeter
   //Serial.println(cdata); 
   
   delay(1000);
   cdata = ""; 

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

  if(dustDensity > 620)
  {
    Serial.println("HIGH");
    }
  else{
    Serial.println("LOW");
    }
  
  delay(1000);

}
