# Week 10
## Goals
For this week, our objectives were the following:
* Redesign the detect_quadrant() methods for each RGB sensor to better differentiate between colors.
* Redesign the sense_blocks() method to track and move toward a block that matches the starting quadrant.
* Model and fabricate a new chassis for the robot.

## Detecting Quadrants
For the past few weeks, we were having trouble programming adequate conditions for each possible color the robot could drive across. We decided to change the conditions by using simplified logic that (for the most part) did not depend on specific numerical values returned by our RGB sensors. The following code snippet highlights this change.

```c++
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
```
For red, blue, and green, it is easy to discern what color the RGB sensor is detecting by comparing the three colors to each other. For example, if the robot is passing over the red quadrant, it is expected that the red value returned is greater than both the green and blue values. Yellow was a bit more complex, but a relatively simple solution was found as well.

## Sensing Blocks
With our detect_quadrant_left()/right() methods working properly, we were able to focus on reimplementing the sense_blocks() method. There were a few problems with the previous implementation. To access the blocks captured by the Pixycam, we used the pixy.blocks.signature method, which returns an array. We originally thought that the array were ordered from largest object on screen to smallest. This is not the case. The array is ordered by color signature (1 to 4, where blue = 1, yellow = 2, green = 3, and red = 4). With a better understanding of how the methods of the Pixy API operated, we were successful in sensing blocks that matched our home quadrant and navigating toward them. The code for this is included below.

```c++
void sense_blocks() {
  //initialize variable to store number of objects Pixycam sees
  int num_blocks = pixy.getBlocks();
  //get left RGB sensor values (temporary)
  uint16_t clear, red, green, blue, lux;
  tcs1.getRawData(&red, &green, &blue, &clear);
  lux = tcs1.calculateLux(red, green, blue);
  //check each object detected by pixycam to see if it
  //matches the target color (and robot is not out of bounds)
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
```
This implementation of sense_blocks() needs a few changes before being considered complete. There is currently an issue involving the robot going out of bounds even though there is a condition in place to stop that from happening. Additionally, this method will need to work with our block-holding mechanism to not only move to the desired block, but also 'lasso' the block and drive it back to the home quadrant.

## A New Chassis
We decided that it would be optimal if our robot has a moving block-capturing mechanism because there were some cases in which the static mechanism would fail. Chiefly, our concern was when the robot went out of bounds with a block. Sure, our robot would remain in bounds, but the block would be lost, and that's one less block we could capture. So we opted to take a creative approach and use a lasso-like block mechanism to capture blocks. 

The new body was built to be bigger and more robust than the previous one, and allows us to install the additional battery board if the need arises. It also allows us to place the PixyCam closer to the ground, which affords our robot the ability to sense blocks using the PixyCam without angling the camera downward, something our previous design did not allow. We were also able to better integrate the color sensors in a way that gives them a proper seat and insulates their wires better (which will be important in the head-to-head rounds). All in all, this new design allows us to both see and capture blocks better, and with its increased size, allows us to increase scalability.

## Next Steps
For the upcoming week, our focus is on rewiring the Arduino and breadboard for the new chassis along with programming the sense_blocks() method to include block-moving functionality. Once completed, we will look to other milestones that have yet to be completed and find ways to implement a more formalized approach to traversing the playing field (based on which quadrant the robot is in) ahead of the competition.

## Pictures
|![Front View](images/week10/Front_View.jpg "Front View")
|:--:|
| *Front View* |
|![Top-down view](images/week10/Top_View.jpg "Top-down view")
| *Top-down view* |
|![Side View](images/week10/Side_View.jpg "Side View")
| *Side View* |
|![Front Mechanism](images/week10/Front_Mechanism.jpg "Front_Mechanism")
| *Front Mechanism* |

## Acknowledgments
Amy and Frank worked on the programming-based objectives for the week. John designed and printed the chassis that we are currently using.
