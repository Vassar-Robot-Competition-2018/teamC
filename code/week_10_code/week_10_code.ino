/*week_10_code_ino
 * Includes functionality for detecting quadrants with two RGB sensors
 * and sensing blocks using the Pixycam. We currently need to fix how
 * the sense_block() method uses RGB sensor values to avoid moving the
 * robot out of bounds. Block moving functionality is also next on the
 * feature list.
 */

#include <Servo.h> //Servo library
#include <SPI.h> //SPI library - to communicate w/ SPI devices
#include <Pixy.h> //Pixycam library
#include <Wire.h> //Wire library - to communicate with I2C devices
#include <Adafruit_TCS34725softi2c.h> // RGB sensor I2C library

//Initialize Pixy object
Pixy pixy;

//constant variable declarations
const int SERVO_1 = 31; //left servo
const int SERVO_2 = 12; //right servo
const int LED_PIN = 22; //LED for sense_blocks()
const int IR_FRONT = A3;
const int BORDER = 80;
const int BLOCK = 170;
const int GREEN_PIN = 5; // Green RGB LED pin
const int BLUE_PIN = 6; // Blue RGB LED pin
const int RED_PIN = 7; // Red RGB LED pin

//quadrant state values (same as Pixy signatures)
const int BLUE = 1;
const int YELLOW = 2;
const int GREEN = 3;
const int RED = 4;

//initial quadrant state set to 0
int target_color = 0;

//initialize two servo objects for the connected servos
Servo servo_test_1;
Servo servo_test_2;

//Digital pins for emulating SDA / SCL
#define SDApin1 34 // left sensor
#define SCLpin1 36 // left sensor
#define SDApin2 35 // right sensor
#define SCLpin2 37 // right sensor

// Initialize the RGB sensors
Adafruit_TCS34725softi2c tcs1 = Adafruit_TCS34725softi2c(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X, SDApin1, SCLpin1);
Adafruit_TCS34725softi2c tcs2 = Adafruit_TCS34725softi2c(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X, SDApin2, SCLpin2);

void setup() {
  Serial.begin(9600);
  tcs1.begin();
  tcs2.begin();
  //initializing pixy
  pixy.init();
  //attaching servos
  servo_test_1.attach(SERVO_1); // attach the signal pin of servo to pin44 of arduino
  servo_test_2.attach(SERVO_2); // attach the signal pin of servo to pin45 of arduino
  // set each pin on the RGB LED to output
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  //set LED to output
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  drive();
  detect_quadrant_left();
  detect_quadrant_right();
  //border_left();
  //border_right();
  sense_blocks();
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

void detect_quadrant_left() {
  //getting values from left RGB sensor
  uint16_t clear, red, green, blue, colorTemp, lux;
  tcs1.getRawData(&red, &green, &blue, &clear);
  lux = tcs1.calculateLux(red, green, blue);
  //until robot reaches tape, drive
  if (target_color == 0) {
    drive();
    //when lux is above 2000, the sensor is on the tape
    if (lux > 2000) {
      //yellow tape conditions
      if ((red > blue) && (green > blue) && ((red - blue) > 1000)) {
        //set the RGB LED to yellow
        setColor(0, 150, 255);
        //change state to yellow
        target_color = YELLOW;
      }
      //white tape conditions
      else if ((red > 9000) && (blue > 9000) && (green > 9000)) {
        // back up, wait, then rotate
        reverse();
        delay(1000);
        rotate();
        delay(2000);
        return;
      }
      //green tape conditions
      else if ((green > red) && (green > blue)) {
        //set the RGB LED to green
        setColor(255, 100, 255);
        //change state to green
        target_color = GREEN;
      }
      //red tape conditons
      else if ((red > green) && (red > blue)) {
        //set the RGB LED to red
        setColor(0, 255, 255);
        //change state to red
        target_color = RED;
      }
      //blue tape conditions
      else if ((blue > green) && (blue > red)) {
        //set the RGB LED to blue
        setColor(255, 255, 0);
        //change state to blue
        target_color = BLUE;
      }
    }
  }
}

void detect_quadrant_right() {
  //getting values from the right RGB sensor
  uint16_t clear, red, green, blue, lux;
  tcs2.getRawData(&red, &green, &blue, &clear);
  lux = tcs2.calculateLux(red, green, blue);
  //until robot reaches tape, drive
  if (target_color == 0) {
    drive();
    //when lux is above 2000, the sensor is on the tape
    if (lux > 2000) {
      //yellow tape conditions
      if ((red > blue) && (green > blue) && ((red - blue) >= 1000)) {
        //set the RGB LED to yellow
        setColor(0, 150, 255);
        //change state to yellow
        target_color = YELLOW;
      }
      //white tape conditions
      else if ((red > 9000) && (blue > 9000) && (green > 9000)) {
        // back up, wait, then rotate
        reverse();
        delay(1000);
        rotate();
        delay(2000);
        return;
      }
      //green tape conditions
      else if ((green > red) && (green > blue)) {
        //set the RGB LED to green
        setColor(255, 100, 255);
        //change state to green
        target_color = GREEN;
      }
      //red tape conditions
      else if ((red > green) && (red > blue)) {
        //set the RGB LED to red
        setColor(0, 255, 255);
        //change state to red
        target_color = RED;
      }
      //blue tape conditions
      else if ((blue > green) && (blue > red)) {
        //set the RGB LED to blue
        setColor(255, 255, 0);
        //change state to blue
        target_color = BLUE;
      }
    }
  }
}

void border_left() {
  //get left RGB sensor values
  uint16_t clear, red, green, blue, lux;
  tcs1.getRawData(&red, &green, &blue, &clear);
  lux = tcs1.calculateLux(red, green, blue);
  //white tape conditions
  if ((red > 9000) && (blue > 9000) && (green > 9000) && (lux > 3000)) {
    // white tape
    Serial.println("Reversing!");
    // back up, wait, then rotate
    reverse();
    delay(1000);
    rotate();
    delay(2000);
    return;
  }
}

void border_right() {
  //get right RGB sensor values
  uint16_t clear, red, green, blue, lux;
  tcs2.getRawData(&red, &green, &blue, &clear);
  lux = tcs2.calculateLux(red, green, blue);
  //white tape conditions
  if ((red > 6700) && (blue > 8100) && (green > 8700) && (lux > 2000)) {
    // back up, wait, then rotate
    reverse();
    delay(1000);
    rotate();
    delay(2000);
    return;
  }
}

void sense_blocks() {
  //intialize variable to store number of objects Pixycam sees
  int num_blocks = pixy.getBlocks();
  //get left RGB sensor values (temporary)
  uint16_t clear, red, green, blue, lux;
  tcs1.getRawData(&red, &green, &blue, &clear);
  lux = tcs1.calculateLux(red, green, blue);
  /*check each object detected by pixycam to see if it
     matches thae target color (and robot is not out of bounds)
  */
  for (int i = 0; i < num_blocks; i++) {
    if ((pixy.blocks[i].signature == target_color) & !((red > 9000) && (blue > 9000) && (green > 9000) && (lux > 3000))) {
      //signal that a block matching target color was found
      signal_block();
      //if object is to the left, turn left
      if (pixy.blocks[i].x < 130) {
        servo_test_1.write(45);
        servo_test_2.write(110);
        delay(250);
      }
      //if object is in central view, drive straight
      else if ((pixy.blocks[i].x >= 130) && (pixy.blocks[i].x <= 160)) {
        servo_test_1.write(45);
        servo_test_2.write(135);
        delay(250);
      }
      //if object is to the right, turn right
      else if (pixy.blocks[i].x > 160) {
        servo_test_1.write(70);
        servo_test_2.write(135);
        delay(250);
      }
    }
    //if there are no blocks matching the target color, stop (temporary)
    else {
      servo_test_1.write(90);
      servo_test_2.write(90);
    }
  }
}

void signal_block() {
  digitalWrite(LED_PIN, HIGH);
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

