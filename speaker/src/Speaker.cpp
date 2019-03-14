#include "speaker/speaker_node.h"
#include "speaker/Speaker.h"
#include "SFML/Audio.hpp"
#include "ros/ros.h"
#include <string>

extern Speaker* speaker;

void speaker_callback(const std_msgs::String::ConstPtr& ptr)
{
	speaker->requestFor(ptr->data);
}

void timer_callback(const ros::TimerEvent& e)
{
	if (speaker->request) {
		speaker->speak();
		speaker->current = "";
		speaker->request = false;
	}
}

Speaker::Speaker()
	: current(""), request(false), after(false)
{
	speaker_subs = handle.subscribe("speaker", 1, speaker_callback);
	timer = handle.createTimer(ros::Duration(0.1), timer_callback);
}

Speaker::~Speaker()
{

}

void Speaker::init()
{

}

void Speaker::speak()
{
	static sf::Music music;
	static sf::Music::Status status;
/*
	if ((status = music.getStatus()) == sf::Music::Playing && this->after) music.stop();
*/
	if ((status = music.getStatus()) == sf::Music::Playing)
		return;

	if (!music.openFromFile(current.c_str())) {
		ROS_INFO("ERROR: Failed to load file.");
		return;
	}

	music.play();
}

void Speaker::requestFor(std::string filename)
{

	if (filename.find("after") != std::string::npos)
		this->after = true;

	this->current = filename;
	this->request = true;
}
