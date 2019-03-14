#include "vision_yolo_ai.h"
#include "std_msgs/Float64MultiArray.h"
#include "sensor_msgs/Image.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>


int sock;

int main(int argc, char* argv[])
{
	ros::init(argc, argv, "vision_yolo_ai");
	ros::NodeHandle nh;

	struct sockaddr_in clntaddr;
	socklen_t clntlen = sizeof(clntaddr);

	memset(&clntaddr, 0, sizeof(clntaddr));
	
	char buf[32];
	sock = openSocket(10234);
	
	ros::Publisher pubs = nh.advertise<std_msgs::Float64MultiArray>("ai", 1);
	//ros::Subscriber subs = nh.subscribe("/camera/depth/image", 1, depth_callback);

	ros::Rate rate(10);
	std_msgs::Float64MultiArray f;

	while (ros::ok()) {
		read(sock, buf, sizeof(buf));
		//recvfrom(sockfd, buf, 32, 0, (struct sockaddr*) &clntaddr, &clntlen);
		
		buf[12] = '\0';

		f.data.push_back(atof(&buf[0]));
		f.data.push_back(atof(&buf[13]));

		ROS_INFO("CENTER: %f, %f", f.data[0], f.data[1]);

		pubs.publish(f);
		f.data.clear();

		rate.sleep();
		ros::spinOnce();
	}

	close(sock);

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


/*
	int sock;
	struct sockaddr_in servaddr;
	socklen_t servlen;

	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		ROS_INFO("SOCKET error");
		exit(-1);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(portno);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	servlen = sizeof(servaddr);

	if (bind(sock, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0) {
		ROS_INFO("BIND error");
		exit(-1);
	}

	return sock;
*/
}
