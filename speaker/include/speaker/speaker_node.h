#ifndef __SPEAKER_NODE_H__
#define __SPEAKER_NODE_H__

#include "std_msgs/String.h"

void speaker_callback(const std_msgs::String::ConstPtr& ptr);

#endif
