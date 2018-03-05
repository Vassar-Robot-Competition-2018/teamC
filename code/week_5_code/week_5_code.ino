/* Parts of this code were adapted from the Adafruit TCS34725 test code
 *  (https://github.com/adafruit/Adafruit_TCS34725) and the 
 *  Adafruit Learn Arduino sketch
 *  (https://learn.adafruit.com/adafruit-arduino-lesson-3-rgb-leds/arduino-sketch)
 */

#include <Servo.h> //Servo library
#include <SPI.h> // to communicate with SPI devices
#include <Pixy.h> // Pixy library
#include <Adafruit_TCS34725.h> // RGB Color Sensor

//constant variable declarations
const int SERVO_1 = 44; //left servo
const int SERVO_2 = 45; //right servo
const int LED_PIN = 38; //LED used to show that a block was sensed
const int IR_FRONT = A3; //front IR sensor
const int IR_BOTTOM = A0; //bottom IR sensor
const int BORDER = 80; //IR value of white reflective tape
const int BLOCK = 300; //IR value of a sensed block
const int GREEN_PIN = 5; // Green RGB LED pin
const int BLUE_PIN = 6; // Blue RGB LED pin
const int RED_PIN = 7; // Red RGB LED pin

//initialize two servo objects for the connected servos
Servo servo_test_1;
Servo servo_test_2;

//initialize Pixy cam
Pixy pixy;

// Initialize the RGB sensor
Adafruit_TCS34725 tcs = Adafruit_TCS34725();


void setup() {
  Serial.begin(9600);
  servo_test_1.attach(SERVO_1); // attach the signal pin of servo to pin44 of arduino
  servo_test_2.attach(SERVO_2); // attach the signal pin of servo to pin45 of arduino
  // set each pin on the RGB LED to output
  pinMode(BLUE_PIN, OUTPUT); 
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
}

void loop() {
// call the drive() and found_color() methods
drive();
found_color();
}

void found_color() {
  uint16_t r, g, b, c, colorTemp, lux;

  // calling tcs methods to access color, lux, and temperature values
  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b);
  
  // Yellow
  if (lux > 20) {
    //  when lux is above 20, the sensor is on the tape
    if (((r > b) && (g > b) && (r > g)) && (b < 60) && (colorTemp < 4000)) {
      // yellow tape
      setColor(0, 150, 255);
      Serial.println("Found Yellow!");
    } else if (((colorTemp > 8300) && (colorTemp < 8700)) && ((lux > 100) && (lux < 200))) {
      // white tape
      Serial.println("Reversing!");
      // back up, wait, then rotate
      reverse();
      delay(1000);
      rotate();
      delay(2000);
      return;
    } else if (((g > r) && (g > b) && (b > r)) && ((r < 50) && (b < 50)) && (colorTemp < 6000)) {
      // green tape
      setColor(255, 100, 255);
      Serial.println("Found Green!");
    } else if (((r > g) && (r > b)) && ((g < 40) && (b < 40))) {
      // red tape
      setColor(0, 255, 255);
      Serial.println("Found Red!");
    } else if (((b > g) && (b > r)) && ((r < 50) && (g < 100)) && (lux < 60) && (colorTemp > 40000)){
      // blue tape
      setColor(255, 255, 0);
      Serial.println("Found Blue!");
    }
  }
}

void drive() {
  // drive in a "straight" line
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

