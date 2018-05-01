#include <SPI.h>
#include <Pixy.h>
#include <Servo.h> //Servo library
#include <Wire.h> //Wire library - to communicate with I2C devices
#include <Adafruit_TCS34725softi2c.h> // RGB sensor I2C library


// This is the main Pixy object
Pixy pixy;
//constant variable declarations
const int SERVO_1 = 31;
const int SERVO_2 = 12;
const int SERVO_MICRO = 2;
const int BLOCK = 500;

const int IR_FRONT = A0;

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
void setup()
{
  Serial.begin(9600);
  //Serial.print("Starting...\n");
  servo_test_1.attach(SERVO_1); // attach the signal pin of servo to pin44 of arduino
  servo_test_2.attach(SERVO_2); // attach the signal pin of servo to pin45 of arduino
  micro_serve.attach(SERVO_MICRO);
  micro_serve.write(90);
  tcs1.begin();
  tcs2.begin();
  pixy.init();
}

void loop() {
  // put your main code here, to run repeatedly:
  int front_sensor_val = analogRead(IR_FRONT);
  sense_blocks(front_sensor_val);


}


const int target_color = 4;

//void follow_block() {
//  int num_blocks = pixy.getBlocks();
//  for (int i = 0; i < num_blocks; i++) {
//    if (pixy.blocks[i].signature == TARGET_SIG) {
//      if (pixy.blocks[i].x < 130) {
//        servo_test_1.write(45);
//        servo_test_2.write(110);
//        delay(250);
//      }
//      else if ((pixy.blocks[i].x >= 130) && (pixy.blocks[i].x <= 160)) {
//        servo_test_1.write(45);
//        servo_test_2.write(135);
//        delay(250);
//      }
//      else if (pixy.blocks[i].x > 160) {
//        servo_test_1.write(70);
//        servo_test_2.write(135);
//        delay(250);
//      }
//      pixy.blocks[i].print();
//    }
//    else {
//      servo_test_1.write(90);
//      servo_test_2.write(90);
//    }
//  }
//}

//void sense_blocks(int front_sensor_val) {
//  int num_blocks = pixy.getBlocks();
//  //Serial.println(num_blocks);
//  for (int i = 0; i < num_blocks; i++) {
//    if (pixy.blocks[i].signature == target_color) {
//      while (pixy.blocks[0].signature != target_color) {
//        if (pixy.blocks[i].x < 130) {
//          servo_test_1.write(45);
//          servo_test_2.write(155);
//          delay(100);
//        }
//        else if ((pixy.blocks[i].x >= 130) && (pixy.blocks[i].x <= 160)) {
//          servo_test_1.write(45);
//          servo_test_2.write(135);
//          delay(100);
//        }
//        else if (pixy.blocks[i].x > 160) {
//          servo_test_1.write(25);
//          servo_test_2.write(135);
//          delay(100);
//        }
//        pixy.blocks[i].print();
//      }
//      if (front_sensor_val >= BLOCK) {
//        signal_block();
//      }
//    }
//    else {
//      rotate();
//      delay(500);
//    }
//  }
//}


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
    if ((pixy.blocks[i].signature == target_color)) { //&& !((red > 9000) || (blue > 9000) || (green > 9000) || (lux > 3000))) {
      Serial.println("Found a block/Not on white tape");
      //signal that a block matching target color was found
      //signal_block();
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
  }

  if (front_sensor_val < BLOCK) {
    //has_block = true;
    drop_lasso();
    //drive_home();
  }
}

void drop_lasso() {
  servo_test_1.write(135);
  servo_test_2.write(45);
  delay(1);
  micro_serve.write(0);
}



