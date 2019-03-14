#include "vision_processor/vision_processor_node.h"
#include "opencv2/core/core.hpp"
#include "cv_bridge/cv_bridge.h"
#include "std_msgs/String.h"
#include "std_msgs/Bool.h"
#include "std_msgs/Int32.h"
#include "vision_processor/Person.h"
#include "string.h"
#include "ros/ros.h"

extern VisionController* controller;
extern bool ready;

VisionController::VisionController(float init1, float init2)
	: speed1(init1), speed2(init2), obstacle(10), timer_speaker(0)
{
	nearest_person = new Person;
	callback = false;
}

VisionController::~VisionController()
{
	delete nearest_person;
}

void VisionController::init()
{
	timer = handle.createTimer(ros::Duration(0.1), timer_callback);
	vision_subs = handle.subscribe("/ai", 1, vision_callback);
	depth_subs = handle.subscribe("/camera/depth/image", 1, depth_callback);

	donation_sub = handle.subscribe("donation", 1, donation_callback);

	vision_pubs = handle.advertise<std_msgs::Float64MultiArray>("person_info", 1);
	speaker_pubs = handle.advertise<std_msgs::String>("speaker", 1);
	clap_pub = handle.advertise<std_msgs::Bool>("arms_control", 1);
}

void VisionController::addPerson(float x, float y, float dep)
{
	class Person p;
	p.set(x, y, dep);
	p.life = 17;
	persons.push_back(p);

	if (persons.size() > 10) {
		persons.erase(persons.begin());
	}
	//updatePerson();
}

void VisionController::updatePerson()
{
	if (depth.data == NULL || persons.size() == 0) return;

	bool die[persons.size()] = {false,};

	for (int i = 0; i < persons.size(); i++) {
		float x = persons[i].xpos;
		float y = persons[i].ypos;

		persons[i].life -= 1;
		if (persons[i].life <= 0) {
			die[i] = true;
		}

		if (!die[i]) {
			for (float j = -0.2; j <= 0.2; j += 0.1) {
				float x_coord = (x + j) * 640;
				float y_coord = y * 480;
				if (x_coord < 0 || x_coord > 640) continue;

				float temp = depth.at<float>(cv::Point((int) x_coord, (int) y_coord));
				if (temp < persons[i].depth) {
					persons[i].xpos = x + j;
					persons[i].ypos = y;
					persons[i].depth = temp;
				}
			}
			for (int k = 0; k < i; k++) {
				float x_diff = persons[i].xpos - persons[k].xpos;
				if (x_diff < 0.1 && x_diff > -0.1)
					die[k] = true;
			}

			if (persons[i].depth > 4) {
				die[i] = true;
			}
		}
	}

	int index = 0;
	int len = persons.size();
	for (int i = 0; i < len; i++) {
		if (die[i])
			persons.erase(persons.begin() + index);
		else
			index += 1;
	}
}

void VisionController::apply()
{
	nearest_person->xpos = 0;
	nearest_person->ypos = 0;
	nearest_person->depth = 10;

	for (int i = 0; i < persons.size(); i++) {
		if (nearest_person->depth > persons[i].depth) {
			memcpy(nearest_person, &persons[i], sizeof(class Person));
		}
	}

	//ROS_INFO("Detected Persons: %u", persons.size());
}

void VisionController::detectObstacle()
{
	this->obstacle = 10;

	if (this->depth.data) {
		for (int i = 290; i < 350; i++) {
			for (int j = 460; j < 480; j++) {
				float temp =  this->depth.at<float>(cv::Point(i, j));
				if (temp < this->obstacle)
					this->obstacle = temp;
			}
		}
	}
}

void VisionController::publish()
{
	static std_msgs::Float64MultiArray arr;
	
	arr.data.push_back(nearest_person->xpos);
	arr.data.push_back(nearest_person->ypos);
	arr.data.push_back(nearest_person->depth);
	arr.data.push_back(this->obstacle);

	if (nearest_person->depth < 1) {
		publish_speaker(std::string(""));
		clap();
	}

	vision_pubs.publish(arr);
	ROS_INFO("X Y DEPTH: %f %f %f", nearest_person->xpos, nearest_person->ypos, nearest_person->depth);
	ROS_INFO("Obstacle: %f", this->obstacle);

	arr.data.clear();
}

void VisionController::clap()
{
	std_msgs::Bool b;
	b.data = true;
	clap_pub.publish(b);
}

void VisionController::publish_speaker(std::string filename)
{
	static int status = 0;

	if (timer_speaker == 0) {
		std_msgs::String msg;
		if (status == 0) {
			msg.data = std::string("/home/group4/Music/hanyang_wav.wav");
			timer_speaker = 80;
			status = 1;
		}
		else {
			msg.data = std::string("/home/group4/Music/before_donation.wav");
			timer_speaker = 200;
			status = 0;
		}
		speaker_pubs.publish(msg);
	}
	else {
		timer_speaker -= 1;
	}
}

float VisionController::depthAt(cv::Point point)
{
	return depth.at<float>(point);
}

void VisionController::setDepth(cv::Mat m)
{
	this->depth = m;
}

void vision_callback(const std_msgs::Float64MultiArray::ConstPtr& ptr)
{
	float x = ptr->data[0];
	float y = ptr->data[1];

	if (controller->callback) {
		float dep = controller->depthAt(cv::Point((int)(x * 640), (int)(y * 480)));

		controller->addPerson(x, y, dep);
		controller->callback = false;
	}
}

void depth_callback(const sensor_msgs::Image::ConstPtr& ptr)
{
	controller->callback = true;

	cv_bridge::CvImagePtr imgPtr = cv_bridge::toCvCopy(ptr, ptr->encoding);

	controller->setDepth(imgPtr->image);
}

void timer_callback(const ros::TimerEvent& e)
{
	if (ready) {
		controller->updatePerson();
		controller->apply();
		controller->detectObstacle();
		controller->publish();
	}
}

void donation_callback(const std_msgs::Int32::ConstPtr& ptr)
{
	if (ptr->data == 1) {
		std_msgs::String msg;
		msg.data = std::string("/home/group4/Music/after_donation.wav");
		controller->speaker_pubs.publish(msg);
		controller->timer_speaker = 50;
	}
}
