#ifndef __VISION_YOLO_NODE_H__
#define __VISION_YOLO_NODE_H__

#include "ros/ros.h"
#include "sensor_msgs/Image.h"

int openSocket(int portno);
void depth_callback(const sensor_msgs::Image::ConstPtr& ptr);

#endif
