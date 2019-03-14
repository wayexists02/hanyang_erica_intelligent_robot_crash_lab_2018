#include <ros/ros.h>
#include "vision_processor/vision_processor_node.h"

VisionController* controller;
bool ready = false;

int main(int argc, char* argv[])
{
	ros::init(argc, argv, "vision_processor_node");
	
	controller = new VisionController(0, 0);
	controller->init();
	ready = true;

	ros::Rate rate(10);

	while (ros::ok()) {
		rate.sleep();
		ros::spinOnce();
	}

	return 0;
}
