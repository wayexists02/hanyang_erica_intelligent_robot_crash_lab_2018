#include "dc_controller/dc_controller.h"
#include "ros/ros.h"

extern DC_Controller* controller;

DC_Controller::DC_Controller()
	: speed1(0), speed2(0), turn(0), stop_distance(false), dirFlag(0), toPublish(true),
	  x(0), y(0), dep(10), turn_timer(4.5)
{

}

DC_Controller::~DC_Controller()
{

}

void DC_Controller::init()
{
	motor_pubs = handle.advertise<std_msgs::Float64MultiArray>("motor_theta_dist", 1);
	vision_subs = handle.subscribe("person_info", 1, person_callback);
	hcsr_subs = handle.subscribe("hcsr", 1, hcsr_callback);
	timer = handle.createTimer(ros::Duration(0.1), timer_callback);
}

void DC_Controller::updateSpeed()
{
	if (x == 0 && y == 0) {
		if (turn_timer > 0) {
			turn_timer -= 0.1;
			if (turn_timer < 3) {
				if (dirFlag == 0) toPublish = false;
				else toPublish = true;
				speed1 = 0;
				speed2 = 0;
				dirFlag = 0;
			}
		}
		else {
			if (dirFlag == 4) toPublish = false;
			else toPublish = true;
			speed1 = 16;
			speed2 = 4;
			dirFlag = 4;
		}
	}
	else if (stop_distance || dep < 1 || dep > 4) {
		if (dirFlag == 0) toPublish = false;
		else toPublish = true;

		speed1 = 0;
		speed2 = 0;
		dirFlag = 0;
	}
	else {
		turn_timer = 4.5;
		if (x < 0.10 || x > 0.90) {
			if (dirFlag == 1) toPublish = false;
			else toPublish = true;

			if (x > 0.50) {
				speed1 = 29;
				speed2 = 40;
			}
			else {
				speed1 = 39;
				speed2 = 33;
			}

			dirFlag = 1;
		}
		else if (x < 0.35 || x > 0.65) {
			if (dirFlag == 2) toPublish = false;
			else toPublish = true;
			
			if (x > 0.50) {
				speed1 = 30;
				speed2 = 38;
			}
			else {
				speed1 = 37;
				speed2 = 33;
			}

			dirFlag = 2;
		}
		else {
			if (dirFlag == 3) toPublish = false;
			else toPublish = true;
			speed1 = 35;
			speed2 = 36;
			dirFlag = 3;
		}
	}
}

void DC_Controller::applySpeed()
{
	static std_msgs::Float64MultiArray arr;
	if (toPublish) {
		arr.data.push_back(speed1);
		arr.data.push_back(speed2);

		motor_pubs.publish(arr);
		ROS_INFO("MSG: %f %f", speed1, speed2);
	}
	arr.data.clear();
}

void DC_Controller::setPerson(float x, float y, float dep)
{
	this->x = x;
	this->y = y;
	this->dep = dep;
}

void person_callback(const std_msgs::Float64MultiArray::ConstPtr& ptr)
{
	float x = ptr->data[0];
	float y = ptr->data[1];
	float dep = ptr->data[2];
	float obstacle = ptr->data[3];
	if (obstacle < 1.2)
		controller->stop_distance = true;
	else
		controller->stop_distance = false;

	controller->setPerson(x, y, dep);
}

void hcsr_callback(const std_msgs::Float64::ConstPtr& ptr)
{
	if (ptr->data < 1)
		controller->stop_distance = true;
	else
		controller->stop_distance = false;
}

void timer_callback(const ros::TimerEvent& e)
{
	controller->updateSpeed();
	controller->applySpeed();
}
