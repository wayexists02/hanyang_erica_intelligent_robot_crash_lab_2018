#include "ros/ros.h"
#include "arms_controller/ArmsController.h"
#include "arms_controller/arms_controller_node.h"
#include "std_srvs/Trigger.h"

class ArmsController* controller;

int main(int argc, char* argv[])
{
	ros::init(argc, argv, "arms_controller_node");

	controller = new ArmsController;
	controller->init();

	ros::Rate rate(10);

	while (ros::ok()) {
		rate.sleep();
		ros::spinOnce();
	}

	return 0;
}

void timer_callback(const ros::TimerEvent& e)
{
	//controller->check();
	controller->counter();
	controller->runMotion();
}

void arms_control_callback(const std_msgs::Bool::ConstPtr& ptr)
{
	if (ptr->data)
		controller->requestAction();
	else
		controller->cancelAction();
}

bool trigger_callback(std_srvs::Trigger::Request& req,
		      std_srvs::Trigger::Response& res)
{
	controller->done();	
}
