#include "left_arm/LeftArm.h"
#include "left_arm/left_arm_node.h"
#include "std_msgs/Int32MultiArray.h"
#include <string>

void LeftArm::init()
{
	arms_sub = handle.subscribe("left_arm", 1, arms_callback);
	arms_pub = handle.advertise<std_msgs::Int32MultiArray>(
		"left_arm_oper", 1
	);
}

void LeftArm::setCommand(std::string command)
{
	static std_msgs::Int32MultiArray arr;
	
	if (command.compare(std::string("idle")) == 0) {
		arr.data.push_back(512);
		arr.data.push_back(448);
		arr.data.push_back(512);
	}
	else if (command.compare(std::string("up")) == 0) {
		arr.data.push_back(256);
		arr.data.push_back(384);
		arr.data.push_back(512);
	}
	else if (command.compare(std::string("clap")) == 0) {
		arr.data.push_back(256);
		arr.data.push_back(576);
		arr.data.push_back(608);
	}

	ROS_INFO("COMMAND: %s", command.c_str());

	arms_pub.publish(arr);
	arr.data.clear();
}
