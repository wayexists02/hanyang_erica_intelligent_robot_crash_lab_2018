#include "ros/ros.h"
#include "dc_controller/dc_controller.h"

DC_Controller* controller;

int main(int argc, char* argv[])
{
	ros::init(argc, argv, "dc_controller_node");

	controller = new DC_Controller();
	controller->init();

	ros::Rate rate(10);

	while (ros::ok()) {
		rate.sleep();
		ros::spinOnce();
	}

	return 0;
}
