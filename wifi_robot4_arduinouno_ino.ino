#define ENA_m2 10       // Enable/speed motor Front Left
#define ENB_m2 11       // Enable/speed motor Back Left
#define IN_21  8        // L298N #2 in 1 motor Front Left
#define IN_22  9        // L298N #2 in 2 motor Front Left
#define IN_23  12       // L298N #2 in 3 motor Back Left
#define IN_24  13       // L298N #2 in 4 motor Back Left

int command;            //Int to store app command state.
int speedCar = 1023;     // 50 - 255.
int speed_Coeff = 3;

int state = 0;

const int TRIG_PIN = 16; // Arduino pin connected to Ultrasonic Sensor's TRIG pin
const int ECHO_PIN = 17; // Arduino pin connected to Ultrasonic Sensor's ECHO pin
float duration_us, distance_cm;

int led = 5;


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

    pinMode(ENA_m2, OUTPUT);
    pinMode(ENB_m2, OUTPUT);
    pinMode(IN_21, OUTPUT);
    pinMode(IN_22, OUTPUT);
    pinMode(IN_23, OUTPUT);
    pinMode(IN_24, OUTPUT);

    digitalWrite(IN_21, LOW);
    digitalWrite(IN_22, LOW);
    digitalWrite(IN_23, LOW);
    digitalWrite(IN_24, LOW);

    pinMode(TRIG_PIN, OUTPUT); // set arduino pin to output mode
    pinMode(ECHO_PIN, INPUT);  // set arduino pin to input mode

    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);
  
    pinMode(ledPower,OUTPUT);
} 

void goAhead(){ 
      digitalWrite(IN_21, LOW);
      digitalWrite(IN_22, HIGH);
      analogWrite(ENA_m2, speedCar);
      
      digitalWrite(IN_23, LOW);
      digitalWrite(IN_24, HIGH);
      analogWrite(ENB_m2, speedCar);

  }

void goBack(){ 

      digitalWrite(IN_21, HIGH);
      digitalWrite(IN_22, LOW);
      analogWrite(ENA_m2, speedCar);

      digitalWrite(IN_23, HIGH);
      digitalWrite(IN_24, LOW);
      analogWrite(ENB_m2, speedCar);

  }

void goRight(){ 

      digitalWrite(IN_21, HIGH);
      digitalWrite(IN_22, LOW);
      analogWrite(ENA_m2, speedCar);

      digitalWrite(IN_23, LOW);
      digitalWrite(IN_24, HIGH);
      analogWrite(ENB_m2, speedCar);


  }

void goLeft(){
      
      digitalWrite(IN_21, LOW);
      digitalWrite(IN_22, HIGH);
      analogWrite(ENA_m2, speedCar);

      digitalWrite(IN_23, HIGH);
      digitalWrite(IN_24, LOW);
      analogWrite(ENB_m2, speedCar);
      
        
  }

void goAheadRight(){

      digitalWrite(IN_21, LOW);
      digitalWrite(IN_22, HIGH);
      analogWrite(ENA_m2, speedCar);

      digitalWrite(IN_23, HIGH);
      digitalWrite(IN_24, LOW);
      analogWrite(ENB_m2, speedCar);
 
  }

void goAheadLeft(){

      digitalWrite(IN_21, LOW);
      digitalWrite(IN_22, HIGH);
      analogWrite(ENA_m2, speedCar);

      digitalWrite(IN_23, HIGH);
      digitalWrite(IN_24, LOW);
      analogWrite(ENB_m2, speedCar);
 
  }

void goBackRight(){ 

      digitalWrite(IN_21, HIGH);
      digitalWrite(IN_22, LOW);
      analogWrite(ENA_m2, speedCar);

      digitalWrite(IN_23, LOW);
      digitalWrite(IN_24, HIGH);
      analogWrite(ENB_m2, speedCar);

  }

void goBackLeft(){ 
  
      digitalWrite(IN_21, HIGH);
      digitalWrite(IN_22, LOW);
      analogWrite(ENA_m2, speedCar);


      digitalWrite(IN_23, LOW);
      digitalWrite(IN_24, HIGH);
      analogWrite(ENB_m2, speedCar);

  }

void stopRobot(){  

      digitalWrite(IN_21, LOW);
      digitalWrite(IN_22, LOW);
      analogWrite(ENA_m2, speedCar);

      digitalWrite(IN_23, LOW);
      digitalWrite(IN_24, LOW);
      analogWrite(ENB_m2, speedCar);

      analogWrite(led, 0);
  
  }

void Auto(){
  analogWrite(led, 250);
  // generate 10-microsecond pulse to TRIG pin
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // measure duration of pulse from ECHO pin
  duration_us = pulseIn(ECHO_PIN, HIGH);
  // calculate the distance
  distance_cm = 0.017 * duration_us;
  int distance = round(distance_cm);
  
  Serial.print("distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(200);
  
  int SensorValue = digitalRead(19);
  
  Serial.print("SensorPin Value: ");
  Serial.println(SensorValue);
  delay(300);
  
   if ((distance <= 5) || (SensorValue == 1)){ // LOW MEANS Object Detected
      //Serial.println("LOW");
      goBack();
  }
  else
  {
      //Serial.println("HIGH");
      goAhead();
  }
  delay(100);



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
void Circle()
{
  goRight();
  delay(5000);
  goAhead();
  delay(1000);
  }
void zigzag()
{
  // generate 10-microsecond pulse to TRIG pin
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // measure duration of pulse from ECHO pin
  duration_us = pulseIn(ECHO_PIN, HIGH);
  // calculate the distance
  distance_cm = 0.017 * duration_us;
  int distance = round(distance_cm);
  
  //Serial.print("distance: ");
  Serial.print(distance);
  //Serial.println(" cm");
  delay(200);
  
  int SensorValue = digitalRead(19);
  
  goAhead();
  
  //Serial.print("SensorPin Value: ");
  //Serial.println(SensorValue);
  delay(300);
  
   if ((distance <= 5) || (SensorValue == 1)){ // LOW MEANS Object Detected
      goBack();
      delay(3000);
      goRight();
      delay(5000);
  }
  
  
  }
void loop(){

  if (Serial.available() > 0){ 
  command = Serial.read();
  
      if (command == 'F') state = 2;
      else if (command == 'B') state = 3;
      else if (command == 'L') state = 4;
      else if (command == 'R') state = 5;
      //else if (command == 'I') goAheadRight();
      else if (command == 'G') goAheadLeft();
      else if (command == 'J') goBackRight();
      else if (command == 'H') goBackLeft();
      else if (command == '0') speedCar = 400;
      else if (command == '1') speedCar = 470;
      else if (command == '2') speedCar = 540;
      else if (command == '3') speedCar = 610;
      else if (command == '4') speedCar = 680;
      else if (command == '5') speedCar = 750;
      else if (command == '6') speedCar = 820;
      else if (command == '7') speedCar = 890;
      else if (command == '8') speedCar = 960;
      else if (command == '9') speedCar = 1023;
      else if (command == 'S') state = 6;

      else if (command == 'A') state = 1;
      
      else if (command == 'P') state = 0;
      
      else if (command == 'O') state = 7;
      else if (command == 'M') state = 8;
      else if (command == 'C') state = 9;
      else if (command == 'I') state = 10;
      else if (command == 'U') state = 11;
      }

if(state == 1)
{
  Auto();
  }
  else if(state == 2)
  {
    goAhead();
    }
    else if(state == 3)
  {
    goBack();
    }
    else if(state == 4)
  {
    goLeft();
    }
    else if(state == 5)
  {
    goRight();
    }
    else if(state == 6)
  {
    stopRobot();
    }
    else if(state == 7)
  {
    highest();
    }
    else if(state == 8)
  {
    medium();
    }
    else if(state == 9)
  {
    lowest();
    }
    else if(state == 10)
  {
    Circle();
    }
    else if(state == 11)
  {
    zigzag();
    }
  else
  {
      stop_mos();
    }

}