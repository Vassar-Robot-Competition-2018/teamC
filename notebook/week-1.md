# Week 1

## The Problem
For the first week of the competition, our goal was to complete the initial milestone. That is, we wanted to get our robot to start in the center of one of the quadrants and exit the quadrant. To complete this task, we needed to do the following:
* Design and create circuits for our two servo motors to interface with the Arduino microcontroller
* Adapt/design code to control the speed of the servos and the direction they spin
* Create a body to house the Arduino, breadboard, servos, and battery

## Getting Started
Our first objective was to connect the servo motors to the Arduino. This involved wiring connections between our breadboard, the servo motors, and the Arduino itself. Power was supplied from the Arduino to the breadboard. The servo motors' positive and ground wires were connected to the breadboard, while the signal wires were connected to the digital pins on the microcontroller. A visualization of this description in the form of an image of the breadboard is available in the *Pictures* section below. With both servo motors hooked up and receiving power, it was time to program their specific movement.

## Code
The focus was shifted to programming the movement of the servos. The code we used was adapted from the following [source](https://www.allaboutcircuits.com/projects/servo-motor-control-with-an-arduino/). For our purposes, we needed an additional to include an servo object (since we have two) along with a call to the Arduino Servo library in order to use servo-specific functions. For the first milestone, constant motor movement is preferable, so that was implemented as well. Here is a snippet of code showing how the servos are accessed and controlled:
```c++
void setup()
{
  servo_test_1.attach(44); // attach the signal pin of servo to pin44
  servo_test_2.attach(45); // attach the signal pin of servo to pin45
}

void loop()
{                          
    servo_test_1.write(45); //command to rotate the servo to the specified angle
    servo_test_2.write(180);
}
```
The attach function attaches each servo to a pin, allowing for the servos to communicate with the board. After attaching both servos to their respective assigned pin numbers, we call the write function to rotate each servo to a desired angle. This ballistic style of design is intended to complete the milestone by having each servo spin in a way that gets our robot out of the quadrant. In the coming weeks, a more reactive approach to changing motor speeds will likely be more desirable, as the servos can be fine tuned in regards to environmental situations through the use of sensors.

## Designing the Body
With the Arduino and servos communicating, we were ready to craft a body to house our equipment and traverse the playing field. Our current body is designed with two tiers of foamcore board to hold different pieces of the robot. The lower tier of the body includes the battery along with both servo motors. The top and bottom tiers are connected by two walls. These walls are glued to each tier and include cutouts for the servos. On the top of the body sits the Arduino and breadboard. There are a few issues with this current design. The lower tier is too low to the ground and will likely have to be adjusted in order to place sensors under the robot. Additionally, attention will need to be paid in regards to the center of gravity of the robot, as our bot was drifting during test trials. Once we solidify a design idea, we hope to 3D print a new body that is more structurally sound. 


## Pictures
|![Our Robot](images/IMG_1920.jpg "Our Robot")
|:--:|
| *Our Robot* |
|![Top-down view](images/IMG_1936.jpg "Top-down view")
| *Top-down view* |
|![Closeup of the breadboard](images/IMG_1915.jpg "Closeup of the breadboard")
| *Closeup of the breadboard* |
|![Front view](images/IMG_1922.jpg "Front view (minus the battery)")
| *Front view (minus the battery) * |

### Acknowledgments
Amy helped build the circuit used to power the servos and connect them to the Arduino.
Frank helped implement code used for controlling the function of the servo motors.
John also helped with programming along with the design and fabrication of the foamcore robot body.
