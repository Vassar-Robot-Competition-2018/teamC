
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

const int BLUE = 1;
const int YELLOW = 2;
const int GREEN = 3;
const int RED = 4;


int target_color = 0;

//initialize two servo objects for the connected servos
Servo servo_test_1;
Servo servo_test_2;

//initialize Pixy cam
Pixy pixy;


// Initialize the RGB sensor
Adafruit_TCS34725 tcs = Adafruit_TCS34725();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
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
  detect_quadrant();
  border();
  int front_sensor_val = analogRead(IR_FRONT);
  sense_blocks(front_sensor_val);


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


void detect_quadrant() {
  uint16_t r, g, b, c, colorTemp, lux;

  // calling tcs methods to access color, lux, and temperature values
  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b);

  if (target_color == 0) {
    drive();


    // Yellow
    if (lux > 20) {
      //  when lux is above 20, the sensor is on the tape
      if (((r > b) && (g > b) && (r > g)) && (b < 60) && (colorTemp < 4000)) {
        // yellow tape
        setColor(0, 150, 255);
        target_color = YELLOW;
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
        target_color = GREEN;
        Serial.println("Found Green!");
      } else if (((r > g) && (r > b)) && ((g < 40) && (b < 40))) {
        // red tape
        setColor(0, 255, 255);
        target_color = RED;
        Serial.println("Found Red!");
      } else if (((b > g) && (b > r)) && ((r < 50) && (g < 100)) && (lux < 60) && (colorTemp > 40000)) {
        // blue tape
        setColor(255, 255, 0);
        target_color = BLUE;
        Serial.println("Found Blue!");
      }
    }
  }
}

void border() {
  uint16_t r, g, b, c, colorTemp, lux;

  // calling tcs methods to access color, lux, and temperature values
  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b);


  if (((colorTemp > 8300) && (colorTemp < 8700)) && ((lux > 100) && (lux < 200))) {
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

void sense_blocks(int front_sensor_val) {
  int num_blocks = pixy.getBlocks();
  //Serial.println(num_blocks);
  for (int i = 0; i < num_blocks; i++) {
    if (pixy.blocks[i].signature == target_color) {
      while (pixy.blocks[0].signature != target_color) {
        if (pixy.blocks[i].x < 130) {
          servo_test_1.write(45);
          servo_test_2.write(155);
          delay(100);
        }
        else if ((pixy.blocks[i].x >= 130) && (pixy.blocks[i].x <= 160)) {
          servo_test_1.write(45);
          servo_test_2.write(135);
          delay(100);
        }
        else if (pixy.blocks[i].x > 160) {
          servo_test_1.write(25);
          servo_test_2.write(135);
          delay(100);
        }
      }
      if (front_sensor_val >= BLOCK) {
        signal_block();
      }
    }
    else {
      rotate();
      delay(500);
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






