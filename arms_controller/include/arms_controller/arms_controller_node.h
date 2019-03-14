#ifndef __ARMS_CONTROLLER_NODE_H__
#define __ARMS_CONTROLLER_NODE_H__

#include "ros/ros.h"
#include "std_msgs/Bool.h"
#include "std_srvs/Trigger.h"

void timer_callback(const ros::TimerEvent& e);
void arms_control_callback(const std_msgs::Bool::ConstPtr& ptr);
bool trigger_callback(std_srvs::Trigger::Request& req,
		      std_srvs::Trigger::Response& res);

#endif
