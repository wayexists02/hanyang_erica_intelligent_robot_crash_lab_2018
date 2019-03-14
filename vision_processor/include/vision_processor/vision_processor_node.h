#ifndef __VISION_PROCESSOR_NODE_H__
#define __VISION_PROCESSOR_NODE_H__

#include "opencv2/core/core.hpp"
#include "sensor_msgs/Image.h"
#include "std_msgs/Float64MultiArray.h"
#include "std_msgs/Int32.h"
#include "ros/ros.h"
#include <vector>
#include <string>

void timer_callback(const ros::TimerEvent& e);
void vision_callback(const std_msgs::Float64MultiArray::ConstPtr& ptr);
void depth_callback(const sensor_msgs::Image::ConstPtr& ptr);
void donation_callback(const std_msgs::Int32::ConstPtr& ptr);

class VisionController {
public:
	VisionController(float initspeed1, float initspeed2);
	virtual ~VisionController();

	void init();
	void addPerson(float x, float y, float dep);
	void updatePerson();
	void apply();
	void publish();
	float depthAt(cv::Point point);
	void setDepth(cv::Mat m);
	void detectObstacle();
	void publish_speaker(std::string filename);
	void clap();

	bool callback;
	int timer_speaker;
	ros::Publisher speaker_pubs;

private:
	float speed1;
	float speed2;
	float obstacle;
	cv::Mat depth;
	std::vector<class Person> persons;
	ros::NodeHandle handle;
	ros::Subscriber vision_subs;
	ros::Subscriber depth_subs;
	ros::Subscriber donation_sub;
	ros::Publisher vision_pubs;
	ros::Publisher clap_pub;
	ros::Timer timer;
	class Person* nearest_person;
};

#endif
