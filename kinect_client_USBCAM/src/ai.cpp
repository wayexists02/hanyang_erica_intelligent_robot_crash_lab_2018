#include "ros/ros.h"
#include "ai.h"

static bool lock;
static unsigned int height;
static unsigned int width;
static unsigned int step;

void setVariables(unsigned int h,
				  unsigned int w,
				  unsigned int s)
{
	if (!lock) {
		ROS_INFO("RECEIVED MSG:");
		ROS_INFO("\tHeight: %u", h);
		ROS_INFO("\tWidth: %u", w);
		ROS_INFO("\tStep: %u", s);

		height = h;
		width = w;
		step = s;

		lock = true;
	}
}

void processImage(const std::vector<unsigned char>& data)
{
	if (lock) {
		// TODO: deep learning

		lock = false;
	}
}
