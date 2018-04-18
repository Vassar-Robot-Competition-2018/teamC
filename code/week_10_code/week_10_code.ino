
#include <Servo.h> //Servo library
#include <SPI.h>
#include <Pixy.h>
#include <Wire.h>
//#include <SoftwareWire.h>
// #include <Adafruit_TCS34725.h> // RGB Color Sensor
#include <Adafruit_TCS34725softi2c.h>

Pixy pixy;

//constant variable declarations
const int SERVO_1 = 31;
const int SERVO_2 = 12;
const int LED_PIN = 22;
const int IR_FRONT = A3;
const int BORDER = 80;
const int BLOCK = 170;
const int GREEN_PIN = 5; // Green RGB LED pin
const int BLUE_PIN = 6; // Blue RGB LED pin
const int RED_PIN = 7; // Red RGB LED pin

const int BLUE = 1;
const int YELLOW = 2;
const int GREEN = 3;
const int RED = 4;


int target_color = GREEN;

//initialize two servo objects for the connected servos
Servo servo_test_1;
Servo servo_test_2;

//initialize Pixy cam



// You can use any digital pin for emulate SDA / SCL
#define SDApin1 35 // right sensor
#define SCLpin1 37 // right sensor
#define SDApin2 34 // left sensor
#define SCLpin2 36 // left sensor

// Initialize the RGB sensors
Adafruit_TCS34725softi2c tcs1 = Adafruit_TCS34725softi2c(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X, SDApin1, SCLpin1);
Adafruit_TCS34725softi2c tcs2 = Adafruit_TCS34725softi2c(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X, SDApin2, SCLpin2);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  tcs1.begin();
  tcs2.begin();
  pixy.init();
  servo_test_1.attach(SERVO_1); // attach the signal pin of servo to pin44 of arduino
  servo_test_2.attach(SERVO_2); // attach the signal pin of servo to pin45 of arduino
  // set each pin on the RGB LED to output
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //stop();
  //detect_quadrant_left();
  //Serial.println(target_color);
  //detect_quadrant_right();
  //Serial.println(target_color);
  //border_left();
  //border_right();
  sense_blocks();
}

void drive() {
  // drive in a "straight" line
  //Serial.println("Driving!");
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

void stop() {
  servo_test_1.write(90);
  servo_test_2.write(90);
}

void detect_quadrant_left() {
  uint16_t clear, red, green, blue, colorTemp, lux;
  tcs2.getRawData(&red, &green, &blue, &clear);
  lux = tcs2.calculateLux(red, green, blue);
  if (target_color == 0) {
    drive();
    // Yellow
    Serial.println(lux);
    if (lux > 2000) {
      Serial.println("Got lux");
      //Serial.println(lux);
      Serial.print("Red: ");
      Serial.println(red);
      Serial.print("Green: ");
      Serial.println(green);
      Serial.print("Blue: ");
      Serial.println(blue);
      //  when lux is above 3000, the sensor is on the tape
      if ((red > blue) && (green > blue) && ((red - blue) > 1000)) {
        // yellow tape
        setColor(0, 150, 255);
        target_color = YELLOW;
        Serial.println("Found Yellow!");
      } else if ((red > 9000) && (blue > 9000) && (green > 9000)) {
        // white tape
        Serial.println("Reversing!");
        // back up, wait, then rotate
        reverse();
        Serial.println("Left Sensor detects white");
        //stop();
        //        Serial.println(red);
        //        Serial.println(green);
        //        Serial.println(blue);
        delay(1000);
        rotate();
        delay(2000);
        return;
      } else if ((green > red) && (green > blue)) {
        // green tape
        setColor(255, 100, 255);
        target_color = GREEN;
        Serial.println("Found Green!");
      } else if ((red > green) && (red > blue)) {
        // red tape
        setColor(0, 255, 255);
        target_color = RED;
        Serial.println("Found Red!");
      } else if ((blue > green) && (blue > red)) {
        // blue tape
        setColor(255, 255, 0);
        target_color = BLUE;
        Serial.println("Found Blue!");
      }
    }
  }
}

void detect_quadrant_right() {
  uint16_t clear, red, green, blue, lux;
  tcs1.getRawData(&red, &green, &blue, &clear);
  lux = tcs1.calculateLux(red, green, blue);

  if (target_color == 0) {
    drive();
    // Yellow
    Serial.println(lux);
    if (lux > 2000) {
      //Serial.println(lux);
      Serial.print("Red: ");
      Serial.println(red);
      Serial.print("Green: ");
      Serial.println(green);
      Serial.print("Blue: ");
      Serial.println(blue);
      //  when lux is above 3000, the sensor is on the tape
      if ((red > blue) && (green > blue) && ((red - blue) >= 1000)) {
        // yellow tape
        setColor(0, 150, 255);
        target_color = YELLOW;
        Serial.println("Found Yellow!");
      } else if ((red > 9000) && (blue > 9000) && (green > 9000)) {
        // white tape
        Serial.println("Reversing!");
        // back up, wait, then rotate
        //reverse();
        Serial.println("Right Sensor detects white");
        //stop();
        delay(1000);
        rotate();
        delay(2000);
        return;
      } else if ((green > red) && (green > blue)) {
        // green tape
        setColor(255, 100, 255);
        target_color = GREEN;
        Serial.println("Found Green!");
      } else if ((red > green) && (red > blue)) {
        // red tape
        setColor(0, 255, 255);
        target_color = RED;
        Serial.println("Found Red!");
      } else if ((blue > green) && (blue > red)) {
        // blue tape
        setColor(255, 255, 0);
        target_color = BLUE;
        Serial.println("Found Blue!");
      }
    }
  }
}


void border_left() {
  uint16_t clear, red, green, blue, lux;
  Serial.println("established uint16_ts");
  tcs1.getRawData(&red, &green, &blue, &clear);
  Serial.println("got raw data");
  lux = tcs1.calculateLux(red, green, blue);
  Serial.println("got lux");


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
  uint16_t clear, red, green, blue, lux;
  Serial.println("established uint16_ts");
  tcs2.getRawData(&red, &green, &blue, &clear);
  Serial.println("got raw data");
  lux = tcs2.calculateLux(red, green, blue);
  Serial.println("got lux");


  if ((red > 6700) && (blue > 8100) && (green > 8700) && (lux > 2000)) {
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

void sense_blocks() {
  int num_blocks = pixy.getBlocks();
  uint16_t clear, red, green, blue, lux;
  tcs1.getRawData(&red, &green, &blue, &clear);
  lux = tcs1.calculateLux(red, green, blue);
  for (int i = 0; i < num_blocks; i++) {
    if ((pixy.blocks[i].signature == target_color) & !((red > 9000) && (blue > 9000) && (green > 9000) && (lux > 3000))) {
      Serial.println("Pixy sig == targer_color");
      if (pixy.blocks[i].x < 130) {
        servo_test_1.write(45);
        servo_test_2.write(110);
        delay(250);
      }
      else if ((pixy.blocks[i].x >= 130) && (pixy.blocks[i].x <= 160)) {
        servo_test_1.write(45);
        servo_test_2.write(135);
        delay(250);
      }
      else if (pixy.blocks[i].x > 160) {
        servo_test_1.write(70);
        servo_test_2.write(135);
        delay(250);
      }
      pixy.blocks[i].print();
    }
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





