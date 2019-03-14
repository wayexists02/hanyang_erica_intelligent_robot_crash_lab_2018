#ifndef __VISION_YOLO_NODE_H__
#define __VISION_YOLO_NODE_H__

#include "ros/ros.h"
#include "sensor_msgs/Image.h"

void ai_rgb_callback(const sensor_msgs::Image::ConstPtr& ptr);
int openSocket(int portno);

#endif
