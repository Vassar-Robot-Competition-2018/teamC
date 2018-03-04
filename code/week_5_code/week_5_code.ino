#include <Servo.h> //Servo library
#include <SPI.h>
#include <Pixy.h>
#include <Adafruit_TCS34725.h> // RGB Color Sensor

//constant variable declarations
const int SERVO_1 = 44; //left servo
const int SERVO_2 = 45; //right servo
const int LED_PIN = 38; //LED used to show that a block was sensed
const int IR_FRONT = A3; //front IR sensor
const int IR_BOTTOM = A0; //bottom IR sensor
const int BORDER = 80; //IR value of white reflective tape
const int BLOCK = 300; //IR value of a sensed block

const int RED_PIN = 7;
const int GREEN_PIN = 5;
const int BLUE_PIN = 6;

int state = 0;

//initialize two servo objects for the connected servos
Servo servo_test_1;
Servo servo_test_2;

//initialize Pixy cam
Pixy pixy;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);


void setup() {
  Serial.begin(9600);

  servo_test_1.attach(SERVO_1); // attach the signal pin of servo to pin44 of arduino
  servo_test_2.attach(SERVO_2); // attach the signal pin of servo to pin45 of arduino
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

//  if (tcs.begin()) {
//    Serial.println("Found sensor");
//  } else {
//    Serial.println("No TCS34725 found ... check your connections");
//    while (1);
//  }
}

void loop() {
  servo_test_1.write(45);
  servo_test_2.write(135);
  found_color();
}

void found_color() {
  uint16_t r, g, b, c, colorTemp, lux;
  
  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b);
 // Yellow
  if ((((r > 20000 && r < 21500) && (g > 18000 && g < 19500)) && (b > 5000 && b < 6000)) && state == 0){
    setColor(255,255,0);
    Serial.println("Found Yellow!");
    state = 1;
    // green
  } else if ((((r > 7000 && r < 8000) && (g > 18000 && g < 19500)) && (b > 8000 && b < 9000)) && state == 0){
    setColor(0,255,0);
    Serial.println("Found Green!");
    state = 1;
    // red
  } else if ((((r > 10000 && r < 11500) && (g > 3000 && g < 4000)) && (b > 3000 && b < 4000)) && state == 0){
    setColor(255,0,0);
    Serial.println("Found Red!");
    state = 1;
    // Blue
  } else if ((((r > 5000 && r < 6000) && (g > 15000 && g < 16500)) && (b > 20000 && b < 22000)) && state == 0){
    setColor(0,0,255);
    Serial.println("Found Blue!");
    state = 1;
  } else {
    Serial.println("I didn't find anything.");
    }
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

