#include <SoftwareWire.h>

/*week_11_code_ino
 Includes functionality for detecting quadrants with two RGB sensors
 and sensing blocks using the Pixycam. We currently need to fix how
 the sense_block() method uses RGB sensor values to avoid moving the
 robot out of bounds. Block moving functionality is also next on the
 feature list.
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
const int SERVO_MICRO = 2; //'lasso' servo
const int LED_PIN = 22; //LED for sense_blocks()
const int IR_FRONT = A0;
const int BLOCK = 500;
const int RED_PIN = 8; // Red RGB LED pin
const int GREEN_PIN = 9; // Green RGB LED pin
const int BLUE_PIN = 10; // Blue RGB LED pin

//quadrant state values (same as Pixy signatures)
const int BLUE_Q = 1;
const int YELLOW_Q = 2;
const int GREEN_Q = 3;
const int RED_Q = 4;

//initial quadrant state set to 0
int target_color = 4;
int current_color = 0;

// set to true when the robot has a block in its possession
boolean has_block = false;

//initialize two servo objects for the connected servos
Servo servo_test_1;
Servo servo_test_2;
Servo micro_serve;

//Digital pins for emulating SDA / SCL
#define SDApin1 34 // left sensor
#define SCLpin1 36 // left sensor
#define SDApin2 35 // right sensor
#define SCLpin2 37 // right sensor

#define COMMON_ANODE

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
  micro_serve.attach(SERVO_MICRO);
  micro_serve.write(90);
  delay(500);
  // set each pin on the RGB LED to output
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  //set LED to output
  pinMode(LED_PIN, OUTPUT);
  setColor(0, 0, 0);
  Serial.println("Setup complete!");
}

void loop() {
  drive();
  //detect_quadrant_left();
  //detect_quadrant_right();
  //  if ((detect_quadrant_left() > 0) && (detect_quadrant_right() == 0)) {
  //    straighten_left();
  //    delay(1000);
  //  }
  //  else if ((detect_quadrant_left() == 0) && (detect_quadrant_right() > 0)) {
  //    straighten_right();
  //    delay(1000);
  //  }
  //
  //
  //  border_left();
  //  border_right();
  int front_sensor_val = analogRead(IR_FRONT);
  if (has_block == false){
    sense_blocks(front_sensor_val);
  }
  //get_sensor_value(front_sensor_val);

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

void kill_servos() {
  servo_test_1.write(90);
  servo_test_2.write(90);
}

int detect_quadrant_left() {
  //getting values from left RGB sensor
  uint16_t clear, red, green, blue, colorTemp, lux;
  tcs1.getRawData(&red, &green, &blue, &clear);
  lux = tcs1.calculateLux(red, green, blue);
  //until robot reaches tape, drive
  //when lux is above 2000, the sensor is on the tape
  //  Serial.println("red");
  //  Serial.println(red);
  //  Serial.println("blue");
  //  Serial.println(blue);
  //  Serial.println("green");
  //  Serial.println(green);
  if (lux > 2000) {
    //Serial.println("LUX > 2000");
    //yellow tape conditions
    if ((red > blue) && (green > blue) && ((red - blue) > 1000)) {
      //set the RGB LED to yellow
      setColor(255, 50, 0);
      //change state to yellow
      if (target_color == 0) {
        target_color = YELLOW_Q;
        Serial.println("Target color");
        Serial.println(target_color);
      }
      current_color = YELLOW_Q;
      Serial.println("Current color");
      Serial.println(current_color);
      return current_color;
    }
    //white tape conditions
    else if ((red > 9000) && (blue > 9000) && (green > 9000)) {
      // back up, wait, then rotate
      reverse();
      delay(1000);
      rotate();
      delay(2000);
      return current_color;
    }
    //green tape conditions
    else if ((green > red) && (green > blue)) {
      //set the RGB LED to green
      setColor(0, 255, 0);
      //change state to green
      if (target_color == 0) {
        target_color = GREEN_Q;
        Serial.println("Target color");
        Serial.println(target_color);
      }
      current_color = GREEN_Q;
      Serial.println("Current color");
      Serial.println(current_color);
      return current_color;
    }
    //red tape conditions
    else if ((red > green) && (red > blue)) {
      //set the RGB LED to red
      setColor(255, 0, 0);
      //change state to red
      if (target_color == 0) {
        target_color = RED_Q;
        Serial.println("Target color");
        Serial.println(target_color);
      }
      current_color = RED_Q;
      Serial.println("Current color");
      Serial.println(current_color);
      return current_color;
    }
    //blue tape conditions
    else if ((blue > green) && (blue > red)) {
      //set the RGB LED to blue
      setColor(0, 0, 255);
      //change state to blue
      if (target_color == 0) {
        target_color = BLUE_Q;
        Serial.println("Target color");
        Serial.println(target_color);
      }
    }
    current_color = BLUE_Q;
    Serial.println("Current color");
    Serial.println(current_color);
    return current_color;
  }
}

int detect_quadrant_right() {
  //getting values from left RGB sensor
  uint16_t clear, red, green, blue, colorTemp, lux;
  tcs2.getRawData(&red, &green, &blue, &clear);
  lux = tcs2.calculateLux(red, green, blue);
  //until robot reaches tape, drive
  //when lux is above 2000, the sensor is on the tape
  //  Serial.println("red");
  //  Serial.println(red);
  //  Serial.println("blue");
  //  Serial.println(blue);
  //  Serial.println("green");
  //  Serial.println(green);
  if (lux > 2000) {
    //Serial.println("LUX > 2000");
    //yellow tape conditions
    if ((red > blue) && (green > blue) && ((red - blue) > 1000)) {
      //set the RGB LED to yellow
      setColor(255, 50, 0);
      //change state to yellow
      if (target_color == 0) {
        target_color = YELLOW_Q;
        Serial.println("Target color");
        Serial.println(target_color);
      }
      current_color = YELLOW_Q;
      Serial.println("Current color");
      Serial.println(current_color);
      return current_color;
    }
    //white tape conditions
    else if ((red > 9000) && (blue > 9000) && (green > 9000)) {
      // back up, wait, then rotate
      reverse();
      delay(1000);
      rotate();
      delay(2000);
      return current_color;
    }
    //green tape conditions
    else if ((green > red) && (green > blue)) {
      //set the RGB LED to green
      setColor(0, 255, 0);
      //change state to green
      if (target_color == 0) {
        target_color = GREEN_Q;
        Serial.println("Target color");
        Serial.println(target_color);
      }
      current_color = GREEN_Q;
      Serial.println("Current color");
      Serial.println(current_color);
      return current_color;
    }
    //red tape conditions
    else if ((red > green) && (red > blue)) {
      //set the RGB LED to red
      setColor(255, 0, 0);
      //change state to red
      if (target_color == 0) {
        target_color = RED_Q;
        Serial.println("Target color");
        Serial.println(target_color);
      }
      current_color = RED_Q;
      Serial.println("Current color");
      Serial.println(current_color);
      return current_color;
    }
    //blue tape conditions
    else if ((blue > green) && (blue > red)) {
      //set the RGB LED to blue
      setColor(0, 0, 255);
      //change state to blue
      if (target_color == 0) {
        target_color = BLUE_Q;
        Serial.println("Target color");
        Serial.println(target_color);
      }
    }
    current_color = BLUE_Q;
    Serial.println("Current color");
    Serial.println(current_color);
    return current_color;
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

void sense_blocks(int front_sensor_val) {
  //initialize variable to store number of objects Pixycam sees

  int num_blocks = pixy.getBlocks();
  //get left RGB sensor values (temporary)
  uint16_t clear, red, green, blue, lux;
  tcs1.getRawData(&red, &green, &blue, &clear);
  lux = tcs1.calculateLux(red, green, blue);
  /*check each object detected by pixycam to see if it
   matches thae target color (and robot is not out of bounds)
   */
  Serial.println(front_sensor_val);
  for (int i = 0; i < num_blocks; i++) {
    //Serial.println(pixy.blocks[i].signature);
    //Serial.println('text');
    if ((pixy.blocks[i].signature == target_color)) { //&& !((red > 9000) || (blue > 9000) || (green > 9000) || (lux > 3000))) {
      Serial.println("Found a block/Not on white tape");
      //signal that a block matching target color was found
      //signal_block();
      //if object is to the left, turn left
      if (pixy.blocks[i].x < 130) {
        servo_test_1.write(90);
        servo_test_2.write(135);
        delay(50);
      }
      //if object is in central view, drive straight
      else if ((pixy.blocks[i].x >= 130) && (pixy.blocks[i].x <= 160)) {
        servo_test_1.write(45);
        servo_test_2.write(135);
        delay(100);
      }
      //if object is to the right, turn right
      else if (pixy.blocks[i].x > 160) {
        servo_test_1.write(45);
        servo_test_2.write(90);
        delay(50);
      }
    }
    //if there are no blocks matching the target color, stop (temporary)
  }

  if (front_sensor_val < BLOCK) {
    //has_block = true;
    drop_lasso();
    has_block = false;
    //drive_home();
  }
}

void drop_lasso() {
  servo_test_1.write(135);
  servo_test_2.write(45);
  delay(50);
  micro_serve.write(1);
  delay(1000);
  kill_servos();
  delay(1000);
}

void lift_lasso() {
  micro_serve.write(90);
}

void get_sensor_value(int front_sensor_val) {
  Serial.println(front_sensor_val);
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

void straighten_left() {
  while ((detect_quadrant_left() > 0) && (detect_quadrant_right() == 0)) {
    servo_test_1.write(90);
    servo_test_2.write(135);
    delay(100);
  }
}

void straighten_right() {
  while ((detect_quadrant_left() == 0) && (detect_quadrant_right() > 0)) {
    servo_test_1.write(45);
    servo_test_2.write(90);
    delay(5);
  }
}

void drive_home() {
  while (current_color != target_color) {
    drive();
    border_left();
    border_right();
    straighten_left();
    straighten_right();
  }
  delay(1000);
  kill_servos();
  lift_lasso();
}




//void model() {
// int home_q = target_color;
//  switch (home_q) {
//    case 1: return_to_blue();
//            break;
//    case 2: return_to_yellow();
//            break;
//    case 3: return_to_green();
//            break;
//    case 4: return_to_red();
//            break;
//  }
//}
//
//void return_to_blue() {
//  int cur_q = current_color;
//  switch (cur_q) {
//
//  }
//}



