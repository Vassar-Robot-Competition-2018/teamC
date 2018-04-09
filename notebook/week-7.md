# Week 7

## Goals
The primary goal for this week was to progress toward completing Milestone 7. That is, we needed to find and signal that we found a block in the playing field that matches our starting quadrant. To do this, we had to modify our existing code to include further block-sensing capabilities. Our secondary goal was to add the ability to move blocks in order to progress further in the competition.

## Milestone 7
The existing code was updated to allow for state-based behavior. A new target_color variable was added to change the behavior of the robot based on which quadrant the bot started in. Our detect_quadrant() function (previously sense_border) was modified to change the target_color to the color of the first line it drives over. The sense_blocks function was also overhauled, using the target_color value and PixyCam information to guide the movement of the robot, as shown in the code below:

```c++
void sense_blocks(int front_sensor_val) {
  int num_blocks = pixy.getBlocks();
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
```

In this method, we check to see if the block(s) captured by the PixyCam match our target_color. While the largest block on screen (first element of the pixy.blocks array) is not the one we are looking for, we move the direction of the robot to the left or right depending on where the other captured blocks are. If we are in close enough range that our IR distance sensor captures the block, we will signal that we found a block. Otherwise, we rotate and run the method again. At this time, we are unsure if this method will work as written since we were also modifying the chassis at the same time, prohibiting testing.
Once we are able to verify that we can accurately sense a block that matches our home quadrant, the next step is to move the block back to where we started. A foreseeable challenge is having the robot align with the block to move it with repeated accuracy.

## Moving Blocks
To move blocks, we designed two curved wedges that connect to the front of the robot on either side of the long-range IR sensor (pictured below). The curved edge of the wedge was designed to ensure that blocks are "guided" more easily towards the IR (which would be the region that holds blocks). These wedges also serve a dual purpose. Beneath each wedge is an RGB sensor, which ensures that no matter how we approach the white tape while driving forward, the tape would be detected and our robot will not go out of bounds. 

Ideally, these wedges should be a simple method of capturing blocks. Tests will determine if we need to make this addition to our robot more complicated, or if this solution is adequate for this task.

## Next Steps
Our current focus is to run our current code on the robot and troubleshoot any potential issues that arise. Once completed, we can start to work on code that allows us to move blocks around the playing field.

## Acknowledgments
John worked on designing and printing the 3D parts needed for providing our robot with proper block-moving abilities. Amy and Frank worked on programming the detect_quadrant() and sense_blocks() methods to add further capabilities.
