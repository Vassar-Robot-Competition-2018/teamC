#include <SPI.h>
#include <Pixy.h>
#include <Servo.h> //Servo library

// This is the main Pixy object
Pixy pixy;
//constant variable declarations
const int SERVO_1 = 31;
const int SERVO_2 = 12;

Servo servo_test_1;
Servo servo_test_2;
void setup()
{
  Serial.begin(9600);
  //Serial.print("Starting...\n");
  servo_test_1.attach(SERVO_1); // attach the signal pin of servo to pin44 of arduino
  servo_test_2.attach(SERVO_2); // attach the signal pin of servo to pin45 of arduino
  pixy.init();
}

void loop() {
  // put your main code here, to run repeatedly:
  follow_block();



}


const int TARGET_SIG = 3;

void follow_block() {
  int num_blocks = pixy.getBlocks();
  for (int i = 0; i < num_blocks; i++) {
    if (pixy.blocks[i].signature == TARGET_SIG) {
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






