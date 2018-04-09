# Week 8

## Goals
Like last week, our goal for this week was to complete Milestone 7. In order to run the code we developed last week, we worked on getting all of our components (sensors, LEDs, pixy cam, etc) set up on our new frame. Most significantly, we added an additional RGB sensor to fine tune our border detection.

## Rewiring 
In order to start implementing the code we wrote last week, we rewired all of the components that were detatched to construct our new body over spring break. This included adding a blue LED in addition to the RGB LED we had before. For milestone 7, the RGB LED will be used to signal what the starting quadrant, and the blue LED will flash when the robot has detected a block of that color. 

## A New RGB Sensor
Most of our time this week was spent troubleshooting issues with the additional RGB sensor. This included figuring out how to connect 2 sensors with only one SDA and one SCL pin on our board. This was solved by using Adafruit_TCS34725_SoftI2C instead of Adafruit_TCS34725 for our RGB sensors (this allowed us to convert digital pins to SDA and SCL pins, in order to attach both sensors). However, we are now having issues detecting color temp as a result, and will need to figure out a solution before we can continue with milestone 7. This is some of the code we used this week to track down where issues in our code were arising:

```c++
void detect_quadrant_left() {
  uint16_t r, g, b, c, colorTemp, lux;
  Serial.println("established uint16_ts");
  tcs1.getRawData(&r, &g, &b, &c);
  Serial.println("got raw data");
  colorTemp = tcs1.calculateColorTemperature(r, g, b);
  Serial.println("calculated color temp");
  lux = tcs1.calculateLux(r, g, b);
  Serial.println("got lux");
```

In this method, we print comments to the serial monitor that report on the progress of the program. This is how we found that our new library was unable to calculate color temp. This means that moving forward, we will need to either find a different way to calculate color temp, or figure out how to defferentiate between the quadrants without it. This will probably be our next step in progressing through milestone 7.


## Next Steps
Hopefully in the next week we will get all of our components working like before so we can move on to running and editing our code.

## Pictures


## Acknowledgments
Amy and Frank worked on rewiring the LEDs and sensors, while John finished work on the body of the robot. John, Frank, and Amy all worked on adding the new RGB sensor and updating the code to be compatable with 2 RGB sensors.
