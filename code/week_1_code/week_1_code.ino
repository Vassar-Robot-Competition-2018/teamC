// This code was adapted from: https://www.allaboutcircuits.com/projects/servo-motor-control-with-an-arduino/

include <Servo.h> //Servo library

//initialize two servo objects for the connected servos
Servo servo_test_1;   
Servo servo_test_2;               
 
void setup() 
{ 
  servo_test_1.attach(44); // attach the signal pin of servo to pin44 of arduino
  servo_test_2.attach(45); // attach the signal pin of servo to pin45 of arduino
} 
  
void loop() 
{                          
    servo_test_1.write(45); //command to rotate the servo to the specified angle
    servo_test_2.write(180);
    delay(15);                       
} 
 
