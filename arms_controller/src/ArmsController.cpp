#include "arms_controller/ArmsController.h"
#include "arms_controller/arms_controller_node.h"
#include "std_srvs/Trigger.h"
#include "std_msgs/String.h"

void ArmsController::init()
{
	task.push_back("up");
	task.push_back("clap");
	task.push_back("up");
	task.push_back("clap");
	task.push_back("up");
	task.push_back("clap");
	task.push_back("idle");

	right_arms_pub = handle.advertise<std_msgs::String>(
		"right_arm", 1
	);
	left_arms_pub = handle.advertise<std_msgs::String>(
		"left_arm", 1
	);

	arms_control_sub = handle.subscribe("arms_control", 1, arms_control_callback);
	//arms_done_client = handle.serviceClient<std_srvs::Trigger>("arms_done");
	arms_done_server = handle.advertiseService("arms_done", trigger_callback);

	timer = handle.createTimer(ros::Duration(0.1), timer_callback);
}

void ArmsController::requestAction()
{
	static std_msgs::String cmd;

	if (this->isDoing) return;

	this->isDoing = true;
	this->finish_one_cycle = true;

	ROS_INFO("Action triggered");
}

void ArmsController::cancelAction()
{
	static std_msgs::String cmd;
	cmd.data = "idle";

	right_arms_pub.publish(cmd);
	left_arms_pub.publish(cmd);
	this->isDoing = false;
	this->finish_one_cycle = true;
	ROS_INFO("Cancel action");
}

void ArmsController::runMotion()
{
	static int index = 0;

	if (!this->isDoing)
		return;

	else if (this->finish_one_cycle) {
		std_msgs::String cmd;
		cmd.data = task[index];

		right_arms_pub.publish(cmd);
		left_arms_pub.publish(cmd);

		index += 1;
		if (index == 7) {
			index = 0;
			this->isDoing = false;
		}
		else {
			this->finish_one_cycle = false;
			this->counter_ = 15;
		}
	}
}

void ArmsController::counter()
{
	if (!this->finish_one_cycle) {
		this->counter_ -= 1;

		if (this->counter_ <= 0) this->finish_one_cycle = true;
	}
}

void ArmsController::done()
{
	//ROS_INFO("A");
	this->finish_one_cycle = true;
}

