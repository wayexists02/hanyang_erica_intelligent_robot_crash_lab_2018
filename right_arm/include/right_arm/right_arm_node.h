#ifndef __RIGHT_ARM_NODE_H__
#define __RIGHT_ARM_NODE_H__

#include "std_msgs/String.h"
#include "ros/ros.h"

void arms_callback(const std_msgs::String::ConstPtr& ptr);

#endif
