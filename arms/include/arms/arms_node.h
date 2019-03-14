#ifndef __ARMS_NODE_H__
#define __ARMS_NODE_H__

#include "std_msgs/Int32MultiArray.h"
#include "ros/ros.h"
#include "std_srvs/Trigger.h"

void right_callback(const std_msgs::Int32MultiArray::ConstPtr& ptr);
void left_callback(const std_msgs::Int32MultiArray::ConstPtr& ptr);
bool check_callback(std_srvs::Trigger::Request& req,
		    std_srvs::Trigger::Response& res);
void timer_callback(const ros::TimerEvent& e);

#endif
