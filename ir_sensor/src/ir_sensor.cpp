#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "libSonar.h"

int main(int argc, char* argv[])
{
    ros::init(argc, argv, "ir_sensor_node");
    ros::NodeHandle nh;
    Sonar sonar;

    int trigger = 0;
    int echo = 1;

    if (wiringPiSetup() < 0) {
		fprintf(stderr, "ERROR: can't initialize wiringPi\n");	
	  	return -1;
    }
    sonar.init(trigger, echo);

    ros::Publisher pub = nh.advertise<std_msgs::Int32>("donation", 1);

    ros::Rate rate(10);

    while (ros::ok()) {
		std_msgs::Int32 b;
		sonar.distance(30000);
		b.data = 1;
		printf("Detected!\n");
		
		pub.publish(b);

		rate.sleep();
		ros::spinOnce();
    }

    return 0;
}
