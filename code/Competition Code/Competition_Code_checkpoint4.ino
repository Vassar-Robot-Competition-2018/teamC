#include <SoftwareWire.h>


/*week_13_code_ino
 Now with block moving functionality!
 */

#include <SPI.h>
#include <Pixy.h>
#include <Servo.h> //Servo library
#include <Wire.h> //Wire library - to communicate with I2C devices
#include <Adafruit_TCS34725softi2c.h> // RGB sensor I2C library


// This is the main Pixy object
Pixy pixy;
//constant variable declarations
const int SERVO_1 = 13;
const int SERVO_2 = 12;
const int SERVO_MICRO = 24;
const int BLOCK = 500;

const int IR_FRONT = A0;
int target_color = 0;
int enemy_color = 0;
int current_color = 0;
int last_color = 0;

const int RED_PIN = 8; // Red RGB LED pin for target color
const int GREEN_PIN = 9; // Green RGB LED pin
const int BLUE_PIN = 10; // Blue RGB LED pin

const int RED_PIN_C = 15; // RGB LED pins for current color
const int GREEN_PIN_C = 16;
const int BLUE_PIN_C = 17;

const int RED_PIN_L = 4; // RGB LED pins for last color
const int GREEN_PIN_L = 3;
const int BLUE_PIN_L = 2;


Servo servo_test_1;
Servo servo_test_2;
Servo micro_serve;


//Digital pins for emulating SDA / SCL
#define SDApin1 34 // left sensor
#define SCLpin1 36 // left sensor
#define SDApin2 35 // right sensor
#define SCLpin2 37 // right sensor

#define COMMON_ANODE

const int BLUE_Q = 1;
const int YELLOW_Q = 2;
const int GREEN_Q = 3;
const int RED_Q = 4;


boolean has_block = false;
boolean has_enemy = false;
boolean has_other = false;
boolean turning_left = false;
boolean turning_right = false;
boolean locked_on = false;

int score = 0;

// Initialize the RGB sensors
Adafruit_TCS34725softi2c tcs1 = Adafruit_TCS34725softi2c(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_4X, SDApin1, SCLpin1);
Adafruit_TCS34725softi2c tcs2 = Adafruit_TCS34725softi2c(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_4X, SDApin2, SCLpin2);
void setup()
{
  Serial.begin(9600);
  //Serial.print("Starting...\n");
  servo_test_1.attach(SERVO_1); // attach the signal pin of servo to pin44 of arduino
  servo_test_2.attach(SERVO_2); // attach the signal pin of servo to pin45 of arduino
  micro_serve.attach(SERVO_MICRO);
  micro_serve.write(180);
  tcs1.begin();
  tcs2.begin();
  pixy.init();
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN_C, OUTPUT);
  pinMode(RED_PIN_C, OUTPUT);
  pinMode(GREEN_PIN_C, OUTPUT);
  pinMode(BLUE_PIN_L, OUTPUT);
  pinMode(RED_PIN_L, OUTPUT);
  pinMode(GREEN_PIN_L, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  drive();
  //  Serial.print("Target Color: "); Serial.print(target_color); Serial.println(" ");
  //  Serial.print("Current Color: "); Serial.print(current_color); Serial.println(" ");
  //  Serial.print("Last Color: "); Serial.print(last_color); Serial.println(" ");
  detect_quadrant_left();
  detect_quadrant_right();
  if (locked_on == false){
    straighten();
  }
  int front_sensor_val = analogRead(IR_FRONT);
  if (has_block == true) {
    drive_home();
  } 
  else if (has_enemy == true) {
    sabotage();
  } 
  else if (has_other == true) {
    mischevious();
  } 
  else {
    sense_blocks(front_sensor_val);
  }
}


void drive() {
  // drive in a "straight" line
  //Serial.println("Driving!!!!!!!!!!!!!!!!!!!!");
  servo_test_1.write(50);
  servo_test_2.write(134);
}

void drive_slow() {
  // drive in a "straight" line
  //Serial.println("Driving!!!!!!!!!!!!!!!!!!!!");
  servo_test_1.write(72);
  servo_test_2.write(112);
}

int determine_enemy(int color){
  if (color == 2){
    enemy_color = 4;
  } 
  else {
    enemy_color = ((color +2) % 4);
  }
}


int detect_quadrant_left() {
  //getting values from left RGB sensor
  uint16_t c, red, green, blue, colorTemp, lux;
  tcs1.getRawData(&red, &green, &blue, &c);
  lux = tcs1.calculateLux(red, green, blue);
  //when lux is above 150, the sensor is on the tape
  if (lux > 150) {
    //yellow tape conditions
    if ((red > blue) && (green > blue) && ((red - blue) > 100)) {
      //set the RGB LED to yellow
      //change state to yellow
      if (target_color == 0) {
        target_color = YELLOW_Q;
        determine_enemy(target_color);
        setColor(255, 150, 0);
      }
      if (current_color != YELLOW_Q) {
        last_color = current_color;
      }
      if (last_color == 1) {
        setcolor_l(0, 0, 255);
      } 
      else if (last_color == 2) {
        setcolor_l(255, 150, 0);
      } 
      else if (last_color == 3) {
        setcolor_l(0, 255, 0);
      } 
      else if (last_color == 4) {
        setcolor_l(255, 0, 0);
      }
      current_color = YELLOW_Q;
      setcolor_c(255, 150, 0);
      return current_color;
    }
    //white tape conditions
    else if ((red > 350) && (blue > 400) && (green > 450)) {
      // back up, wait, then rotate
      reverse();
      delay(1000);
      rotate();
      delay(1000);
      locked_on = false;
      return -1;
    }
    //green tape conditions
    else if ((green > red) && (green > blue) && (c < 550)) {
      //set the RGB LED to green
      //change state to green
      if (target_color == 0) {
        target_color = GREEN_Q;
        determine_enemy(target_color);
        setColor(0, 255, 0);
      }
      if (current_color != GREEN_Q) {
        last_color = current_color;
      }
      if (last_color == 1) {
        setcolor_l(0, 0, 255);
      } 
      else if (last_color == 2) {
        setcolor_l(255, 150, 0);
      } 
      else if (last_color == 3) {
        setcolor_l(0, 255, 0);
      } 
      else if (last_color == 4) {
        setcolor_l(255, 0, 0);
      }
      current_color = GREEN_Q;
      setcolor_c(0, 255, 0);
      return current_color;
    }
    //red tape conditions
    else if ((red > green) && (red > blue)) {
      //set the RGB LED to red
      //change state to red
      if (target_color == 0) {
        target_color = RED_Q;
        determine_enemy(target_color);
        setColor(255, 0, 0);
      }
      if (current_color != RED_Q) {
        last_color = current_color;
      }
      if (last_color == 1) {
        setcolor_l(0, 0, 255);
      } 
      else if (last_color == 2) {
        setcolor_l(255, 150, 0);
      } 
      else if (last_color == 3) {
        setcolor_l(0, 255, 0);
      } 
      else if (last_color == 4) {
        setcolor_l(255, 0, 0);
      }
      current_color = RED_Q;
      setcolor_c(255, 0, 0);
      return current_color;
    }
    //blue tape conditions
    else if ((blue > green) && (blue > red)) {
      //set the RGB LED to blue

      //change state to blue
      if (target_color == 0) {
        target_color = BLUE_Q;
        determine_enemy(target_color);
        setColor(0, 0, 255);
      }
      if (current_color != BLUE_Q) {
        last_color = current_color;
      }
      if (last_color == 1) {
        setcolor_l(0, 0, 255);
      } 
      else if (last_color == 2) {
        setcolor_l(255, 150, 0);
      } 
      else if (last_color == 3) {
        setcolor_l(0, 255, 0);
      } 
      else if (last_color == 4) {
        setcolor_l(255, 0, 0);
      }
      current_color = BLUE_Q;
      setcolor_c(0, 0, 255);
      return current_color;
    }
    else {
      return 0;
    }
  }
  else {
    return 0;
  }
}


int detect_quadrant_right() {
  //getting values from left RGB sensor
  uint16_t c, red, green, blue, colorTemp, lux;
  tcs2.getRawData(&red, &green, &blue, &c);
  lux = tcs2.calculateLux(red, green, blue);
  //until robot reaches tape, drive
  //when lux is above 2000, the sensor is on the tape
  //  Serial.println("red");
  //  Serial.println(red);
  //  Serial.println("blue");
  //  Serial.println(blue);
  //  Serial.println("green");
  //  Serial.println(green);
  if (lux > 150) {
    //Serial.println("LUX > 2000");
    //yellow tape conditions
    if ((red > blue) && (green > blue) && ((red - blue) > 100)) {
      //set the RGB LED to yellow
      //change state to yellow
      if (target_color == 0) {
        target_color = YELLOW_Q;
        determine_enemy(target_color);
        setColor(255, 150, 0);
      }
      if (current_color != YELLOW_Q) {
        last_color = current_color;
      }
      if (last_color == 1) {
        setcolor_l(0, 0, 255);
      } 
      else if (last_color == 2) {
        setcolor_l(255, 150, 0);
      } 
      else if (last_color == 3) {
        setcolor_l(0, 255, 0);
      } 
      else if (last_color == 4) {
        setcolor_l(255, 0, 0);
      }
      current_color = YELLOW_Q;
      setcolor_c(225, 150, 0);
      return current_color;
    }
    //white tape conditions
    else if ((red > 500) && (blue > 550) && (green > 600)) {
      // back up, wait, then rotate
      reverse();
      delay(1000);
      rotate();
      delay(1000);
      locked_on = false;
      return -1;
    }
    //green tape conditions
    else if ((green > red) && (green > blue) && (c < 550)) {
      //set the RGB LED to green
      //change state to green
      if (target_color == 0) {
        target_color = GREEN_Q;
        determine_enemy(target_color);
        setColor(0, 255, 0);
      }
      if (current_color != GREEN_Q) {
        last_color = current_color;
      }
      if (last_color == 1) {
        setcolor_l(0, 0, 255);
      } 
      else if (last_color == 2) {
        setcolor_l(255, 150, 0);
      } 
      else if (last_color == 3) {
        setcolor_l(0, 255, 0);
      } 
      else if (last_color == 4) {
        setcolor_l(255, 0, 0);
      }
      current_color = GREEN_Q;
      setcolor_c(0, 255, 0);
      return current_color;
    }
    //red tape conditions
    else if ((red > green) && (red > blue)) {
      //set the RGB LED to red

      //change state to red
      if (target_color == 0) {
        target_color = RED_Q;
        determine_enemy(target_color);
        setColor(255, 0, 0);
      }
      if (current_color != RED_Q) {
        last_color = current_color;
      }
      if (last_color == 1) {
        setcolor_l(0, 0, 255);
      } 
      else if (last_color == 2) {
        setcolor_l(255, 150, 0);
      } 
      else if (last_color == 3) {
        setcolor_l(0, 255, 0);
      } 
      else if (last_color == 4) {
        setcolor_l(255, 0, 0);
      }
      current_color = RED_Q;
      setcolor_c(255, 0, 0);
      return current_color;
    }
    //blue tape conditions
    else if ((blue > green) && (blue > red)) {
      //set the RGB LED to blue
      //change state to blue
      if (target_color == 0) {
        target_color = BLUE_Q;
        determine_enemy(target_color);
        setColor(0, 0, 255);
      }
      if (current_color != BLUE_Q) {
        last_color = current_color;
      }
      if (last_color == 1) {
        setcolor_l(0, 0, 255);
      } 
      else if (last_color == 2) {
        setcolor_l(255, 150, 0);
      } 
      else if (last_color == 3) {
        setcolor_l(0, 255, 0);
      } 
      else if (last_color == 4) {
        setcolor_l(255, 0, 0);
      }
      current_color = BLUE_Q;
      setcolor_c(0, 0, 255);
      return current_color;
    }
    else {
      return 0;
    }
  }
  else {
    return 0;
  }
}


void sense_blocks(int front_sensor_val) {
  //initialize variable to store number of objects Pixycam sees
  int num_blocks = pixy.getBlocks();
  for (int i = 0; i < num_blocks; i++) {
    detect_quadrant_left();
    detect_quadrant_right();
    if ((pixy.blocks[i].signature == target_color) || (pixy.blocks[i].signature == enemy_color)) {
      locked_on = true;
      //if object is to the left, turn left
      if (pixy.blocks[i].x < 130) {
        servo_test_1.write(80);
        servo_test_2.write(135);
        delay(50);
      }
      //if object is in central view, drive straight
      else if ((pixy.blocks[i].x >= 130) && (pixy.blocks[i].x <= 160)) {
        servo_test_1.write(45);
        servo_test_2.write(135);
        delay(50);
      }
      //if object is to the right, turn right
      else if (pixy.blocks[i].x > 160) {
        servo_test_1.write(45);
        servo_test_2.write(100);
        delay(50);
      }
    }
  }
  if (front_sensor_val < BLOCK) {
    determine_block();
    rotate();
    delay(1300);
  }
}


void determine_block(){
  int num_blocks = pixy.getBlocks();
  for (int i = 0; i < num_blocks; i++) {
    detect_quadrant_left();
    detect_quadrant_right();
//    Serial.println("y value");
//    Serial.println(pixy.blocks[i].y);
    if (((pixy.blocks[i].x > 105) && (pixy.blocks[i].x < 225)) && ((pixy.blocks[i].y > 150) && (pixy.blocks[i].y < 170))){
      Serial.println("Found big block");
      if (pixy.blocks[i].signature == target_color) {
        Serial.println("Found our block");
        //our block
        has_block = true;
        locked_on = false;
        drop_lasso();
      } 
      else if (pixy.blocks[i].signature == enemy_color) {
        // enemy block
        Serial.println("Found enemy block");
        has_enemy = true;
        locked_on = false;
        drop_lasso();
      } 
      else {
        Serial.println("Found other block");
        has_other = true;
        locked_on = false;
        drop_lasso();
      }
    }
  }
}

void drop_lasso() {
  servo_test_1.write(135);
  servo_test_2.write(45);
  delay(100);
  micro_serve.write(90);
  delay(100);
}

void lift_lasso() {
  micro_serve.write(180);
  reverse();
  delay(500);
  rotate();
  delay(1000);
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

void setcolor_c(int red, int green, int blue)
{
#ifdef COMMON_ANODE
  red = 255 - red;
  green = 255 - green;
  blue = 255 - blue;
#endif
  analogWrite(RED_PIN_C, red);
  analogWrite(GREEN_PIN_C, green);
  analogWrite(BLUE_PIN_C, blue);
}

void setcolor_l(int red, int green, int blue)
{
#ifdef COMMON_ANODE
  red = 255 - red;
  green = 255 - green;
  blue = 255 - blue;
#endif
  analogWrite(RED_PIN_L, red);
  analogWrite(GREEN_PIN_L, green);
  analogWrite(BLUE_PIN_L, blue);
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


void straighten() {
  //  if ((detect_quadrant_left() != detect_quadrant_right()) && (detect_quadrant_right() > 0) && (detect_quadrant_left() > 0)){
  //    Serial.println("Straightening right");
  //    servo_test_1.write(80);
  //    servo_test_2.write(45);
  //    delay(500);
  //  } else
  if ((detect_quadrant_left() == 0) && (detect_quadrant_right() > 0)) {
    while ((detect_quadrant_left() != detect_quadrant_right()) && (detect_quadrant_right() == 0 || detect_quadrant_left() == 0)) {
      //Serial.println("Straighten right");
      servo_test_1.write(80);
      servo_test_2.write(45);
      detect_quadrant_left();
      detect_quadrant_right();
    }
  } 
  else if ((detect_quadrant_left() > 0) && (detect_quadrant_right() == 0)){
    while ((detect_quadrant_left() != detect_quadrant_right()) && (detect_quadrant_right() == 0 || detect_quadrant_left() == 0)) {
      //Serial.println("Straighten left");
      servo_test_1.write(135);
      servo_test_2.write(110);
      detect_quadrant_left();
      detect_quadrant_right();
    }
  }
}
void turn_left() {
  servo_test_1.write(135);
  servo_test_2.write(110);
  delay(1000);
}

void turn_right() {
  servo_test_1.write(80);
  servo_test_2.write(45);
  delay(1000);
}

void drive_home() {

  while (current_color != target_color) {
    int current = current_color;
    drive();
    detect_quadrant_left();
    detect_quadrant_right();
    straighten();
    while (detect_quadrant_left() <= 0 && detect_quadrant_right() <= 0) {
      drive();
      Serial.println("Driving towards tape");
      detect_quadrant_left();
      detect_quadrant_right();
      straighten();
    }
    straighten();
    while (current == current_color) {
      drive_slow();
      detect_quadrant_left();
      detect_quadrant_right();
    }
    current = current_color;
    drive();
    detect_quadrant_left();
    detect_quadrant_right();
    delay(1500);  
    if (((current_color == YELLOW_Q && last_color == BLUE_Q) || (current_color == GREEN_Q && last_color == YELLOW_Q) || (current_color == RED_Q && last_color == GREEN_Q) || (current_color == BLUE_Q && last_color == RED_Q)) && (target_color != current_color)) {
      turn_left();
      detect_quadrant_left();
      detect_quadrant_right();
    } 
    else if (((current_color == BLUE_Q && last_color == YELLOW_Q) || (current_color == YELLOW_Q && last_color == GREEN_Q) || (current_color == GREEN_Q && last_color == RED_Q) || (current_color == RED_Q && last_color == BLUE_Q)) && (target_color != current_color)) {
      turn_right();
      detect_quadrant_left();
      detect_quadrant_right();
    } else {
      rotate();
      detect_quadrant_left();
      detect_quadrant_right();
    }
  }
  drive();
  delay(1000);
  lift_lasso();
  has_block = false;
}

void sabotage() {
  // moves enemy blocks
  while ((current_color != target_color) && (current_color != enemy_color)) {
    int current = current_color;
    drive();
    detect_quadrant_left();
    detect_quadrant_right();
    straighten();
    while (detect_quadrant_left() <= 0 && detect_quadrant_right() <= 0) {
      drive();
      Serial.println("Driving towards tape");
      detect_quadrant_left();
      detect_quadrant_right();
      straighten();
    }
    straighten();
    while (current == current_color) {
      drive_slow();
      detect_quadrant_left();
      detect_quadrant_right();
    }
    current = current_color;
    drive();
    detect_quadrant_left();
    detect_quadrant_right();
    delay(1500);  
    if (((current_color == YELLOW_Q && last_color == BLUE_Q) || (current_color == GREEN_Q && last_color == YELLOW_Q) || (current_color == RED_Q && last_color == GREEN_Q) || (current_color == BLUE_Q && last_color == RED_Q)) && (target_color != current_color)) {
      turn_left();
      detect_quadrant_left();
      detect_quadrant_right();
    } 
    else if (((current_color == BLUE_Q && last_color == YELLOW_Q) || (current_color == YELLOW_Q && last_color == GREEN_Q) || (current_color == GREEN_Q && last_color == RED_Q) || (current_color == RED_Q && last_color == BLUE_Q)) && (target_color != current_color)) {
      turn_right();
      detect_quadrant_left();
      detect_quadrant_right();
    } else {
      rotate();
      detect_quadrant_left();
      detect_quadrant_right();
    }
  }
  drive();
  delay(1000);
  lift_lasso();
  has_enemy = false;
}

void mischevious() {
  // moves enemy blocks
  while (current_color != enemy_color) {
    int current = current_color;
    drive();
    detect_quadrant_left();
    detect_quadrant_right();
    straighten();
    while (detect_quadrant_left() <= 0 && detect_quadrant_right() <= 0) {
      drive();
      Serial.println("Driving towards tape");
      detect_quadrant_left();
      detect_quadrant_right();
      straighten();
    }
    straighten();
    while (current == current_color) {
      drive_slow();
      detect_quadrant_left();
      detect_quadrant_right();
    }
    current = current_color;
    drive();
    detect_quadrant_left();
    detect_quadrant_right();
    delay(1500);  
    if (((current_color == YELLOW_Q && last_color == BLUE_Q) || (current_color == GREEN_Q && last_color == YELLOW_Q) || (current_color == RED_Q && last_color == GREEN_Q) || (current_color == BLUE_Q && last_color == RED_Q)) && (target_color != current_color)) {
      turn_left();
      detect_quadrant_left();
      detect_quadrant_right();
    } 
    else if (((current_color == BLUE_Q && last_color == YELLOW_Q) || (current_color == YELLOW_Q && last_color == GREEN_Q) || (current_color == GREEN_Q && last_color == RED_Q) || (current_color == RED_Q && last_color == BLUE_Q)) && (target_color != current_color)) {
      turn_right();
      detect_quadrant_left();
      detect_quadrant_right();
    } else {
      rotate();
      detect_quadrant_left();
      detect_quadrant_right();
    }  
  }
  drive();
  delay(1000);
  lift_lasso();
  has_other = false;
}











