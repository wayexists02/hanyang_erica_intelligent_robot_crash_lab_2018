#include "ros/ros.h"
#include "left_arm/LeftArm.h"
#include "left_arm/left_arm_node.h"

class LeftArm* arm;

int main(int argc, char* argv[])
{
	ros::init(argc, argv, "left_arm_node");

	arm = new LeftArm();
	arm->init();

	ros::Rate rate(10);

	while (ros::ok()) {
		rate.sleep();
		ros::spinOnce();
	}

	return 0;
}

void arms_callback(const std_msgs::String::ConstPtr& ptr)
{
	arm->setCommand(ptr->data);
}
