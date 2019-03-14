#include "ros/ros.h"
#include "right_arm/RightArm.h"
#include "right_arm/right_arm_node.h"

class RightArm* arm;

int main(int argc, char* argv[])
{
	ros::init(argc, argv, "right_arm_node");

	arm = new RightArm();
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
