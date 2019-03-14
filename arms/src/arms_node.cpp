#include "ros/ros.h"
#include "arms/Arms.h"
#include "std_srvs/Trigger.h"
#include "arms/arms_node.h"

class Arms* arms;

int main(int argc, char* argv[])
{
	ros::init(argc, argv, "arms_node");
	ros::NodeHandle nh;

	arms = new Arms;
	arms->init();

	ros::Rate rate(10);

	while (ros::ok()) {
		rate.sleep();
		ros::spinOnce();
	}

	return 0;
}

void right_callback(const std_msgs::Int32MultiArray::ConstPtr& ptr)
{
	int shoulder = ptr->data[0];
	int upper = ptr->data[1];
	int lower = ptr->data[2];

	arms->setRightGoalPosition((uint16_t)shoulder, (uint16_t)upper, (uint16_t)lower);
}

void left_callback(const std_msgs::Int32MultiArray::ConstPtr& ptr)
{	
	int shoulder = ptr->data[0];
	int upper = ptr->data[1];
	int lower = ptr->data[2];

	arms->setLeftGoalPosition((uint16_t)shoulder, (uint16_t)upper, (uint16_t)lower);
}

bool check_callback(std_srvs::Trigger::Request& req,
		    std_srvs::Trigger::Response& res)
{
	if (arms->checkForFinish())
		res.message = "finish";
	else
		res.message = "";

	return true;
}

void timer_callback(const ros::TimerEvent& e)
{
	static int counter = 5;

	if (counter == 0 && arms->checkForFinish()) {
		counter = 5;
		arms->trigger();
	}
	else {
		counter -= 1;
	}
}
