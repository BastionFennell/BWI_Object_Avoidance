How to run our program:

1. turn on the laptop

2. turn on the segway base

3. on the laptop, run 
      $ roslaunch segbot_bringup segbot_hokuyo.launch

4. on the computer, run
      $ roslaunch objectAvoidance object_avoidance.launch --screen
      
5. on the computer, also run
      $ rosrun objectAvoidance avoidance

6. finally, go back to the launch screen from step 4, click inside, and run the teleop-commands as normal. The robot should no longer run into objects.





TODO:

short term:
Let the robot move backwards when stuck.
Add Kinect support.

Long term:
Implement support for sonar and IR sensors for glass and cliff detections. 
Set up the object avoidance system to always intercept messages to cmd velocity.
