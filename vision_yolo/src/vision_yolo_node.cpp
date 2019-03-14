#include "vision_yolo_node.h"
#include "std_msgs/Float64.h"
#include "sensor_msgs/Image.h"
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <arpa/inet.h>
#include "cv_bridge/cv_bridge.h"
#include "opencv2/opencv.hpp"

int sock;

//ros::Publisher pubs;

int main(int argc, char* argv[])
{
	ros::init(argc, argv, "vision_yolo_node");
	ros::NodeHandle nh;

	sock = openSocket(10244);

	//pubs = nh.advertise<sensor_msgs::Image>("depth", 1);
	ros::Subscriber subs_rgb = nh.subscribe("/camera/rgb/image_color", 1, ai_rgb_callback);
	
	ros::spin();

	close(sock);

	return 0;
}

void ai_rgb_callback(const sensor_msgs::Image::ConstPtr& ptr)
{
	int bytes = 0;
	static unsigned char buf[7681];
	cv_bridge::CvImagePtr cvImg;

	ROS_INFO("%u %u %u %lu", ptr->width, ptr->height, ptr->step, ptr->data.size());

	cvImg = cv_bridge::toCvCopy(ptr, ptr->encoding);	
/*
	cv::Mat frame;
	frame = cvImg->image.reshape(0, 1);
	int imgSize = frame.total() * frame.elemSize();	
*/
	//write(sock, cvImg->image.data, 480*640*3);

	struct sockaddr_in servaddr;

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(10244);

	//lseek(sock, SEEK_SET, 0);
	for (int i = 0; i < 120; i++) {
		memset(buf, 0, sizeof(buf));
		buf[0] = i;
		memcpy(&buf[1], &cvImg->image.data[i * 7680], 7680);
		if ((bytes = sendto(sock, buf, 7681,
			0, (struct sockaddr*) &servaddr,
			sizeof(servaddr))) < 0)
			ROS_INFO("SEND FAILED");
	}

	//if (sendto(sock, cvImg->image.data, 480*640*3, 0, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0) ROS_INFO("SEND FAILED");
}

int openSocket(int portno)
{
	int sockfd;

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		ROS_INFO("Socket creation failed.");
		exit(-1);
	}

	return sockfd;
}
