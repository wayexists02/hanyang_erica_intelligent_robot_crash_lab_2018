#ifndef __SPEAKER_H__
#define __SPEAKER_H__

#include "ros/ros.h"
#include <string>
#include "SFML/Audio.hpp"

class Speaker {
public:
	Speaker();
	virtual ~Speaker();

	void init();
	void speak();
	void requestFor(std::string filename);

	std::string current;
	bool request;
	bool after;

private:
	ros::NodeHandle handle;
	ros::Timer timer;
	ros::Subscriber speaker_subs;
};

#endif
