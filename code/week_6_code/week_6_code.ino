
#include <Servo.h> //Servo library
#include <SPI.h>
#include <Pixy.h>
#include <Adafruit_TCS34725.h> // RGB Color Sensor

//constant variable declarations
const int SERVO_1 = 44;
const int SERVO_2 = 45;
const int LED_PIN = 38;
const int IR_FRONT = A3;
const int IR_BOTTOM = A0;
const int BORDER = 80;
const int BLOCK = 170;
const int GREEN_PIN = 5; // Green RGB LED pin
const int BLUE_PIN = 6; // Blue RGB LED pin
const int RED_PIN = 7; // Red RGB LED pin

int state = 0;

//initialize two servo objects for the connected servos
Servo servo_test_1;
Servo servo_test_2;

//initialize Pixy cam
Pixy pixy;


// Initialize the RGB sensor
Adafruit_TCS34725 tcs = Adafruit_TCS34725();

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
  random_number = rand() % 5;
  if ((random_number == 0) || (random_number == 1)) {
    servo_test_1.write(45);
    servo_test_2.write(135);
  }
  else if ((random_number == 2) || (random_number == 3)) {
    servo_test_1.write(90);
    servo_test_2.write(150);
  }
  else if (random_number == 4) {
    servo_test_1.write(180);
    servo_test_2.write(130);
  }
  delay(250);
}

void found_color() {
  uint16_t r, g, b, c, colorTemp, lux;

  // calling tcs methods to access color, lux, and temperature values
  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b);

  // Yellow
  if (((colorTemp > 8000) && (colorTemp < 9000)) && ((lux > 95) && (lux < 200))) {
    // white tape
    Serial.println("Reversing!");
    // back up, wait, then rotate
    reverse();
    delay(1000);
    rotate();
    delay(1000);
    state = 1;
    return;
  }
}

void drive() {
  // drive in a "straight" line
  Serial.println("Driving!");
  servo_test_1.write(45);
  servo_test_2.write(135);
}

void reverse() {
  // back the robot up
  servo_test_1.write(135);
  servo_test_2.write(45);
}

void rotate() {
  // turn the robot
  servo_test_1.write(45);
  servo_test_2.write(45);
}

void sense_blocks(int front_sensor_val) {
  int num_blocks = pixy.getBlocks();
  //Serial.println(num_blocks);
  if ((front_sensor_val >= BLOCK) && (num_blocks >= 1)) {
    Serial.println("Found a block!");
    found_color();
    driving_with_block();
  }
}

void led_on() {
  setColor(0, 0, 0);
}

void led_off() {
  setColor(255, 255, 255);
}

void loop() {
  //timer = millis();
  led_off();
  int front_sensor_val = analogRead(IR_FRONT);
  //Serial.println(front_sensor_val);
  int random_number = rand() % 3;
  //Serial.println(random_number);
  found_color();
  rand_cruise(random_number);
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

void driving_with_block() {
  while (state != 1) {
    found_color();
    drive();
    led_on();
  }
  state = 0;
}

