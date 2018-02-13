#include <Servo.h> //Servo library

const int SERVO_1 = 44;
const int SERVO_2 = 45;
const int IR = A0;
int state = 0;

//initialize two servo objects for the connected servos
Servo servo_test_1;
Servo servo_test_2;

void setup()
{
  Serial.begin(9600);

  servo_test_1.attach(SERVO_1); // attach the signal pin of servo to pin44 of arduino
  servo_test_2.attach(SERVO_2); // attach the signal pin of servo to pin45 of arduino

}

void loop()
{

  int sensor_val = analogRead(IR);
  Serial.println(sensor_val);

  while ((sensor_val >= 80) && (state == 0)) {
    servo_test_1.write(45); //command to rotate the servo to the specified angle
    servo_test_2.write(135);
    delay(250);
    return;
  }
  if ((sensor_val < 80) && (state == 0)) {
    servo_test_1.write(135); //go in reverse
    servo_test_2.write(45);
    sensor_val = 0;
    state = 1;
  }
  if (state == 1) {
    delay(3000);
    servo_test_1.write(90);
    servo_test_2.write(90);
  }
}


