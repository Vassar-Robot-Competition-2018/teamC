
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
const int BLOCK = 150;
const int GREEN_PIN = 5; // Green RGB LED pin
const int BLUE_PIN = 6; // Blue RGB LED pin
const int RED_PIN = 7; // Red RGB LED pin


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
  pinMode(BLUE_PIN, OUTPUT); 
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  pixy.init(); // intialize pixy object
}

void rand_cruise(int random_number) {
  random_number = rand() % 4;
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
  delay(250);
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
  //Serial.println(num_blocks);
  if ((front_sensor_val >= BLOCK) && (num_blocks >= 1)) {
    Serial.println("Found a block!");
    servo_test_1.write(90);
    servo_test_2.write(90);
    led_on();
    delay(2000);
    led_off();
  }
}

void led_on() {
  setColor(0,0,0);
}

void led_off() {
  setColor(255,255,255);
}

void loop() {
  //timer = millis();
  led_off();
  int bottom_sensor_val = analogRead(IR_BOTTOM);
  int front_sensor_val = analogRead(IR_FRONT);
  Serial.println(front_sensor_val);
  int random_number = rand() % 3;
  //Serial.println(random_number);
  rand_cruise(random_number);
  //sense_border(bottom_sensor_val);
  sense_blocks(front_sensor_val);
}


void setColor(int red, int green, int blue)
{
#ifdef COMMON_ANODE
  red = 255 - red;
  green = 255 - green;
  blue = 255 - blue;
#endif
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}
