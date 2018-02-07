// This code was adapted from: https://www.allaboutcircuits.com/projects/servo-motor-control-with-an-arduino/

#include <Servo.h> //Servo library

const int SERVO_1 = 44;
const int SERVO_2 = 45;
const int IR = A0;

//initialize two servo objects for the connected servos
Servo servo_test_1;   
Servo servo_test_2;               
 
void setup() 
{ 
  Serial.begin(9600);
//  servo_test_1.attach(SERVO_1); // attach the signal pin of servo to pin44 of arduino
//  servo_test_2.attach(SERVO_2); // attach the signal pin of servo to pin45 of arduino
} 
  
void loop() 
{                          
//    servo_test_1.write(45); //command to rotate the servo to the specified angle
//    servo_test_2.write(180);
    int sensor_val = analogRead(IR);
    Serial.println(sensor_val);
    delay(500);                       
} 
 
