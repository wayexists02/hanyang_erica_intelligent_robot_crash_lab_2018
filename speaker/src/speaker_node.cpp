#include "ros/ros.h"
#include "speaker/speaker_node.h"
#include "speaker/Speaker.h"

Speaker* speaker;

int main(int argc, char* argv[])
{
	ros::init(argc, argv, "speaker_node");

	speaker = new Speaker;
	speaker->init();

	ros::spin();

	return 0;
}
