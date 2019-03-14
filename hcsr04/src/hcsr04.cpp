#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "libSonar.h"

int main(int argc, char* argv[])
{
    ros::init(argc, argv, "hcsr04_node");
    ros::NodeHandle nh;
    Sonar sonar;

    int trigger = 4;
    int echo = 5;

    if (wiringPiSetup() < 0) {
		fprintf(stderr, "ERROR: can't initialize wiringPi\n");	
	  	return -1;
    }
    sonar.init(trigger, echo);

    ros::Publisher pub = nh.advertise<std_msgs::Float64>("hcsr", 1);

    ros::Rate rate(10);

    while (ros::ok()) {
		std_msgs::Float64 f;

		f.data = sonar.distance(30000);
		printf("Distance: %f\n", f.data);

		pub.publish(f);

		rate.sleep();
		ros::spinOnce();
    }

    return 0;
}
