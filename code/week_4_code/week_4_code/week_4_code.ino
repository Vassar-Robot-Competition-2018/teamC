#include <Servo.h> //Servo library
#include <SPI.h>
#include <Pixy.h>

//constant variable declarations
const int SERVO_1 = 44;
const int SERVO_2 = 45;
const int LED_PIN = 38;
const int IR_FRONT = A3;
const int IR_BOTTOM = A0;
const int BORDER = 80;
const int BLOCK = 130;

//initialize two servo objects for the connected servos
Servo servo_test_1;
Servo servo_test_2;

//initialize Pixy cam
Pixy pixy;

void setup()
{
  Serial.begin(9600);

  servo_test_1.attach(SERVO_1); // attach the signal pin of servo to pin44 of arduino
  servo_test_2.attach(SERVO_2); // attach the signal pin of servo to pin45 of arduino
  pinMode(LED_PIN, OUTPUT);

   pixy.init();
}

void rand_cruise(int random_number) {
  //random_number = rand() % 3;
  //int random_number = 2;
  if (random_number == 0) {
    servo_test_1.write(90);
    servo_test_2.write(150);
  }
  else if (random_number == 1) {
    servo_test_1.write(45);
    servo_test_2.write(135);
  }
  else {
    servo_test_1.write(180);
    servo_test_2.write(130);
  }
}

void sense_border(int bottom_sensor_val) {
  if (bottom_sensor_val < BORDER) {
    servo_test_1.write(135); //go in reverse
    servo_test_2.write(45);
    delay(1000);
    servo_test_1.write(45); //turn around
    servo_test_2.write(90);
    delay(1000);
  }
}

void sense_blocks(int front_sensor_val) {
  int num_blocks = pixy.getBlocks();
  Serial.println(num_blocks);
  //int num_blocks = pixy.blocks[].length;
  //int num_blocks = 5;
  if ((front_sensor_val > BLOCK) && (num_blocks >= 1)) {
    Serial.println("Found a block!");
    servo_test_1.write(90);
    servo_test_2.write(90);
    led_on();
    delay(10000);
  }
}

void led_on() {
  digitalWrite(LED_PIN, HIGH);   // turns the LED on
    delay(1000);
    digitalWrite(LED_PIN, LOW);
    delay(1000);
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
}

void loop() {
  int bottom_sensor_val = analogRead(IR_BOTTOM);
  int front_sensor_val = analogRead(IR_FRONT);
  //Serial.println(front_sensor_val);
  int random_number = rand() % 3;
  //Serial.println(random_number);
  rand_cruise(random_number);
  delay(500);
  sense_border(bottom_sensor_val);
  sense_blocks(front_sensor_val);
  //delay(300);
}

