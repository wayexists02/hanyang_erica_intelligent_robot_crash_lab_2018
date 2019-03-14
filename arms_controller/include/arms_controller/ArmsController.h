#ifndef __ARMS_CONTROLLER_H__
#define __ARMS_CONTROLLER_H__

#include "ros/ros.h"
#include <vector>
#include <string>

class ArmsController {
public:
	ArmsController(): isDoing(false), finish_one_cycle(true) {}
	virtual ~ArmsController() {}

	void init();
	void requestAction();
	void cancelAction();
	//void check();
	void runMotion();
	void done();
	void counter();

private:
	int counter_;
	bool finish_one_cycle;
	bool isDoing;
	std::vector<std::string> task;
	ros::NodeHandle handle;
	ros::Subscriber arms_control_sub;
	ros::Publisher right_arms_pub;
	ros::Publisher left_arms_pub;
	ros::Timer timer;
	ros::ServiceServer arms_done_server;
	//ros::ServiceClient arms_done_client;
};

#endif
