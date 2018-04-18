#include <Servo.h> //Servo library

const int SERVO_1 = 31;
const int SERVO_2 = 12;

Servo servo_test_1;
Servo servo_test_2;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  servo_test_1.attach(SERVO_1); // attach the signal pin of servo to pin44 of arduino
  servo_test_2.attach(SERVO_2); // attach the signal pin of servo to pin45 of arduino
}

void loop() {
  // put your main code here, to run repeatedly:
  servo_test_1.write(45);
  servo_test_2.write(135);
  delay(2000);
//  servo_test_1.write(0);
//  servo_test_2.write(0);
//  delay(2000);
  servo_test_1.write(70);
  servo_test_2.write(135);
  delay(2000);
  servo_test_1.write(45);
  servo_test_2.write(110);
  delay(2000);
  

}
