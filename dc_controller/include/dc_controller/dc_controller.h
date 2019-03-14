#ifndef __DC_CONTROLLER_H__
#define __DC_CONTROLLER_H__

#include "std_msgs/Float64MultiArray.h"
#include "std_msgs/Float64.h"
#include "ros/ros.h"

class DC_Controller
{
public:
	DC_Controller();
	virtual ~DC_Controller();

	void init();
	void applySpeed();
	void updateSpeed();
	void setPerson(float x, float y, float dep);

	bool stop_distance;
	float turn_timer;

private:
	int dirFlag;
	bool toPublish;
	float speed1, speed2;
	float x, y, dep;
	float turn;
	float timer_motor;
	ros::Timer timer;
	ros::Subscriber vision_subs;
	ros::Subscriber hcsr_subs;
	ros::Publisher motor_pubs;
	ros::NodeHandle handle;
};

void person_callback(const std_msgs::Float64MultiArray::ConstPtr& ptr);
void hcsr_callback(const std_msgs::Float64::ConstPtr& ptr);
void timer_callback(const ros::TimerEvent& e);

#endif
