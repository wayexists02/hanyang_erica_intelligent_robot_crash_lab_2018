#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <strings.h>

#include "ros/ros.h"
#include "sensor_msgs/Image.h"
#include "std_msgs/Float64.h"
#include "ai.h"

//ros::Subscriber subs_res;
//ros::Subscriber subs_cam;
//ros::Publisher pubs_img;

float center_x;
float depth;

int openSocket(int portno);

int main(int argc, char* argv[])
{
	ros::init(argc, argv, "kinect_client_node");
	ros::NodeHandle nh;

	int server = openSocket(10234);
	char buf[32];
	//char buf_d[32];

	//subs_res = nh.subscribe("result", 100, callback_res);
	//subs_cam = nh.subscribe("/usb_cam/image_raw", 100, callback_img);
	ros::Publisher pubs = nh.advertise<std_msgs::Float64>("ai", 1);

	//nh.setParam("center_x", 0.5f);
	//nh.setParam("depth", 0.f);

	ros::Rate rate(10);

	while (ros::ok()) {
		std_msgs::Float64 f;

		read(server, buf, sizeof(buf));
		ROS_INFO("CENTER: %s\n", buf);

		f.data = atof(buf);
		
		pubs.publish(f);

		rate.sleep();

		ros::spinOnce();
	}
	close(server);

	return 0;
}

int openSocket(int portno)
{
	int sockfd;
	struct hostent* server;
	struct sockaddr_in serv_addr;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		ROS_INFO("Socket creation failed.");
		exit(-1);
	}

	if ((server = gethostbyname("localhost")) == NULL) {
		ROS_INFO("Failed to get server.");
		exit(-1);
	}

	bzero((char*) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char*) server->h_addr, (char*) &serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);

	if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
		ROS_INFO("Failed to connect");
		exit(-1);
	}

	return sockfd;
}

