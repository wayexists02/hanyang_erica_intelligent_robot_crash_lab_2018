#ifndef __RIGHT_ARM_H__
#define __RIGHT_ARM_H__

#include "ros/ros.h"
#include <string>

class RightArm {
public:
	RightArm() {}
	virtual ~RightArm() {}

	void init();
	void setCommand(std::string command);

private:
	ros::NodeHandle handle;
	ros::Subscriber arms_sub;
	ros::Publisher arms_pub;
};

#endif
