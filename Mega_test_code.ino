#include <SoftwareSerial.h>
#include <HCSR04.h>

HCSR04 centerSonic(28, 29); //initialisation class HCSR04 (trig pin , echo pin)
HCSR04 leftSonic(30, 31); //initialisation class HCSR04 (trig pin , echo pin)
HCSR04 rightSonic(32, 33); //initialisation class HCSR04 (trig pin , echo pin)
HCSR04 backSonic(34, 35); //initialisation class HCSR04 (trig pin , echo pin)



//HERE
#define LM_1 23
#define LM_2 25
#define LM_EN 5
#define RM_1 22
#define RM_2 24
#define RM_EN 3

int moveDir=0; // 1 = (Front, Left or Right) 2 = (Reverse)

void motor_op (char LM_dir, byte LM_SPD, char RM_dir, byte RM_SPD) 
// input parameters (direction of left motor, pwm value of left motor, direction of right motor, pwm value of right motor)
{
  if (LM_dir == 'f')
  {
    digitalWrite(LM_1, LOW);
    digitalWrite(LM_2, HIGH);   
  }
  else if (LM_dir == 'b')
  {
    digitalWrite(LM_1, HIGH);
    digitalWrite(LM_2, LOW);   
  }
  analogWrite(LM_EN,LM_SPD);
  
  if (RM_dir == 'f')
  {
    digitalWrite(RM_1, LOW);
    digitalWrite(RM_2, HIGH);   
  }
  else if (RM_dir == 'b')
  {
    digitalWrite(RM_1, HIGH);
    digitalWrite(RM_2, LOW);   
  }
  analogWrite(RM_EN,RM_SPD);  
}

//HERE

//Started SoftwareSerial at new RX (5) and new TX (6) pin of ESP8266/NodeMCU
SoftwareSerial newSerial(10,9);
SoftwareSerial serialFromSonic(50,51);

int data; //Initialized variable to store recieved data

void setup() {



  pinMode(LM_1, OUTPUT);
  pinMode(LM_2, OUTPUT);
  pinMode(RM_1, OUTPUT);
  pinMode(RM_2, OUTPUT);
  pinMode(LM_EN, OUTPUT);
  pinMode(RM_EN, OUTPUT);

  
  //Serial Begin at 9600 Baud 
  Serial.begin(115200);
  newSerial.begin(115200);
  Serial1.begin(115200);
//  serialFromSonic.begin(115200);
}

void loop() {

  motorControlBasedOnSerial();
//  ultrSonicLogic();

//  Serial.println( "Center"); //return current distance (cm) in serial
//  Serial.println( centerSonic.dist() ); //return current distance (cm) in serial
//  delay (250);
//  Serial.println( "Left"); 
//  Serial.println( leftSonic() ); //return current distance (cm) in serial
//  delay (250);
  
  
//  newSerial.write(123);
//  delay(1000);
}

void ultrSonicLogic() {

  if ((centerSonic.dist() < 10 || leftSonic.dist() < 10) && (moveDir == 1) ){
    motor_op('f',0, 'f', 0);
    moveDir = 0;
  }
  delay(80);

  
  
}

void motorControlBasedOnSerial(){


  if (Serial1.available() > 0){
    int data_sonic = Serial1.read(); //Read the serial data and store it
    Serial.println(data_sonic);
    if ((data_sonic == 1) && (moveDir == 1) ){
    motor_op('f',0, 'f', 0);
    moveDir = 0;
  }


      if ((data_sonic == 2) && (moveDir == 2) ){
          motor_op('f',0, 'f', 0);
          moveDir = 0;
        }
  }

//  centerSonic.dist();
//  Serial.println("Unable to cross this");

//  if ((centerSonic.dist() < 10 || leftSonic.dist() < 10) && (moveDir == 1) ){
//    motor_op('f',0, 'f', 0);
//    moveDir = 0;
//  }
//  delay(100);

  
  if (newSerial.available() > 0) {
//      Serial.println(newSerial.available());
      data = newSerial.read(); //Read the serial data and store it
      Serial.println(data);
  //    delay(1000);
  
      if (data == 50)
      {
      // run motor in forward direction
      motor_op('f',100, 'f', 100);            
//      Serial.println("forward");
      Serial.println("reverse");
      moveDir = 2;
      }
      else   if (data == 10)
      {
        // run motor in reverse direction
        motor_op('b',100, 'b', 100);
//        Serial.println("reverse");
        Serial.println("front");
        moveDir = 1;
      } 
      else   if (data == 70)
      {
        // run motor in left direction
        motor_op('b',180, 'f', 180);
        Serial.println("left");
        moveDir = 1;
      } 
      else   if (data == 90)
      {
        // run motor in right direction
        motor_op('f',180, 'b', 180);
        Serial.println("right");
        moveDir = 1;
      } 
      else   if (data == 30)
      {
        // stop
        motor_op('f',0, 'f', 0);
        Serial.println("stop");
        moveDir = 0;
    
      }    
  }
}
