#include "ros/ros.h"
#include "std_msgs/String.h"
#include <vector>
#include <string>
#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"

using namespace cv;

std::string mode;

void display_callback(const std_msgs::String::ConstPtr& ptr)
{
	mode = ptr->data;
}

int main(int argc, char* argv[])
{
	ros::init(argc, argv, "display_node");
	ros::NodeHandle nh;

	ros::Subscriber sub = nh.subscribe("display", 1, display_callback);

	ros::Rate rate(10);

	Mat img1 = imread("/home/group4/hello.jpg");
	Mat img2 = imread("/home/group4/helpme.jpg");
	Mat img3 = imread("/home/group4/thx.jpg");
	Mat img4 = imread("/home/group4/thx2.jpg");
	Mat m1, m2, m3, m4;

	resize(img1, m1, Size(576, 335), 0, 0, CV_INTER_LINEAR);
	resize(img2, m2, Size(576, 335), 0, 0, CV_INTER_LINEAR);
	resize(img3, m3, Size(576, 335), 0, 0, CV_INTER_LINEAR);
	resize(img4, m4, Size(576, 335), 0, 0, CV_INTER_LINEAR);

	namedWindow("Display", CV_WINDOW_AUTOSIZE);
	moveWindow("Display", 0, 0);
	imshow("Display", m1);
	int ty_state = 0;
	int cnt = 0;

	while (ros::ok()) {
		if (cnt > 0) {
			cnt -= 1;
			continue;
		}

		if (mode.compare(std::string("please")) == 0) {
			imshow("Display", m2);
			cnt = 40;
		}
		else if (mode.compare(std::string("thank")) == 0) {
			if (ty_state == 0) {
				imshow("Display", m3);
				ty_state = 1;
			}
			else {
				imshow("Display", m4);
				ty_state = 0;
			}
		}
		else {
			imshow("Display", m1);
		}
		waitKey(1);
		rate.sleep();
		ros::spinOnce();
	}

	cvDestroyAllWindows();

	return 0;
}
