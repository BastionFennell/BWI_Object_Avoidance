#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "cmvision/Blobs.h"
#include "sensor_msgs/LaserScan.h"
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iostream>
bool safeToMove = true;
ros::Publisher velocity_pub;
//the robot will keep running as long as there are

//Point of interest - check type of parameter
//Hope this works!
void unsafeCallback(const geometry_msgs::Twist& msg){
	
	ROS_INFO("%s", "unsafe callback");
	geometry_msgs::Twist current;
	current.linear.x = 1;
		current.angular.z  = 1;
	
	if(safeToMove)
	{
		std::cout << "TELEOP IS MOVING\n" << std::endl;
		velocity_pub.publish(current);
	}
	else
	{
		std::cout << "STOPPING\n" << std::endl;
		current.linear.x = 1;
		current.angular.z = msg.angular.z;
		velocity_pub.publish(current);
	}
}

void scanCallback(const sensor_msgs::LaserScan& msg){
	
	ROS_INFO("%s", "scanning callback");
	geometry_msgs::Twist stop;
	stop.linear.x = 0;

	//shallow copy, only meant to be pedantic.
	std::vector<float> myvector = msg.ranges;
   

    double stop_Min = msg.range_min;
	double stop_Max = msg.range_max;
	
	std::cout << "scanCallback\n";

    std::cout << "myvector contains:";
	//float min = 999999;
	safeToMove = true;
   for (std::vector<float>::iterator it=myvector.begin(); it!=myvector.end(); ++it){

	//the current value we're iterating through.
        float val = *it;
	
 
	if(val >= stop_Min && val <= stop_Max )
    {
			 
		  if( 0.2 <= val && val <= 0.55){
		  		//safeToMove = false;  
				ROS_INFO("%s", "unsafe callback");
		  		break;
		  }

	}
	}
	double sum = std::accumulate(myvector.begin(), myvector.end(),0.0);
	double mean = sum/myvector.size();
	stop.linear.x = stop.linear.x * (1 - (mean/msg.range_max));
	//velocity_pub.publish(stop);

}


int main(int argc, char **argv){
	
	ros::init(argc, argv, "follower");
	ros::NodeHandle n;
	
	std::cout << "AVOIDANCE IS RUNNING" << std::endl;
	// advertise that we will publish cmd_vel messages
	velocity_pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 1000);

	// subscribe to blob messages and call blobCallback when they are received
	//ros::Subscriber sub = n.subscribe("blobs", 1000, blobCallback);   
	
	ros::Subscriber scanSub = n.subscribe("hokuyo_laser_filters/foo",1000,scanCallback);
	ros::Subscriber teleopSub = n.subscribe("cmd_vel_unsafe",1000,unsafeCallback);
	
	ros::Rate loop_rate(100);

	ros::spin();


	return 0;
}


