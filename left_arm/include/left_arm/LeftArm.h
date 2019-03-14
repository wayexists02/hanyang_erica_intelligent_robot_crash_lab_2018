#ifndef __LEFT_ARM_H__
#define __LEFT_ARM_H__

#include "ros/ros.h"
#include <string>

class LeftArm {
public:
	LeftArm() {}
	virtual ~LeftArm() {}

	void init();
	void setCommand(std::string command);

private:
	ros::NodeHandle handle;
	ros::Subscriber arms_sub;
	ros::Publisher arms_pub;
};

#endif
