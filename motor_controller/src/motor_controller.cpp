#include "ros/ros.h"

#include "std_msgs/Float64MultiArray.h" //theta, distance
#include "std_msgs/String.h"            //done
#include "std_msgs/Float64.h"
#include "sensor_msgs/Image.h"
#include "sensor_msgs/image_encodings.h"
#include "cv_bridge/cv_bridge.h"
#include "opencv2/core/core.hpp"

#include <vector>
#include <cstdlib>
#include <pthread.h>

struct Person {
	float x, y, depth;
};

static bool stop_hcsr, stop;
//static pthread_mutex_t mutex_timer;
//static pthread_mutex_t mutex_person;
//static pthread_mutex_t mutex_xy;
static float timer;
static float x, y;
static float ground_depth;
static std::vector<Person> persons;
static struct Person p;
static ros::Publisher td_pub_ij;

void ai_rgb_callback(const std_msgs::Float64MultiArray::ConstPtr& ptr)
{
	//pthread_mutex_lock(&mutex_timer);
	timer = 1.5;
	//pthread_mutex_unlock(&mutex_timer);
	
	//pthread_mutex_lock(&mutex_xy);
	x = ptr->data[0];
	y = ptr->data[1];
	//pthread_mutex_unlock(&mutex_xy);
}

void ai_depth_callback(const sensor_msgs::Image::ConstPtr& ptr)
{
	float depth_temp;
	//pthread_mutex_lock(&mutex_xy);
	float x_temp = x;
	float y_temp = y;
	//pthread_mutex_unlock(&mutex_xy);

	int depth_x_pos = x_temp * 640;
	int depth_y_pos = y_temp * 480;

	cv_bridge::CvImagePtr cvImg = cv_bridge::toCvCopy(ptr, ptr->encoding);

	depth_temp = cvImg->image.at<float>(cv::Point(depth_x_pos, depth_y_pos));
	
	ground_depth = 10.f;
	for (int i = 0; i < 40; i++) {
		for (int j = 0; j < 60; j++) {
			float temp_ground_depth = cvImg->image.at<float>(cv::Point(290 + j, 400 + i));
			if (temp_ground_depth < ground_depth)
				ground_depth = temp_ground_depth;
		}
	}
	ROS_INFO("GROUND: %f", ground_depth);

	if (timer == 0) return;

	//pthread_mutex_lock(&mutex_person);

	Person p;

	p.x = x_temp;
	p.y = y_temp;
	p.depth = depth_temp;

	persons.push_back(p);
	if (persons.size() > 10)
		persons.erase(persons.begin());

	for (int i = 0; i < persons.size(); i++) {
		depth_x_pos = persons[i].x * 640;
		depth_y_pos = persons[i].y * 480;
		persons[i].depth = cvImg->image.at<float>(cv::Point(depth_x_pos, depth_y_pos));
	}
	//pthread_mutex_unlock(&mutex_person);
}

void hcsr_callback(const std_msgs::Float64::ConstPtr& data)
{
  ROS_INFO("HCSR: %f\n", data->data);

  if (data->data <= 100.) {
    stop_hcsr = true;
  }
  else {
    stop_hcsr = false;
  }
}

float relieveSpeed(float original, float newspeed)
{
	static float accelerate = 0;

	float diff = newspeed - original;

	if (newspeed == 0) {
		return 0;
	}
	else if (diff < -3 || diff > 3) {
		if (diff < -3)
			accelerate = -1;
		else if (diff > 3)
			accelerate = +1;
	}
	else if (diff < -1 || diff > 1) {
		if (diff < -1)
			accelerate = -0.3;
		else if (diff > 1)
			accelerate = +0.3;
	}
	else {
		accelerate = 0.0;
	}

	return original + accelerate;
}

void move(const ros::TimerEvent& event)
{
	static std_msgs::Float64MultiArray arr;
	if (arr.data.size() == 0) {
		arr.data.push_back(0);
		arr.data.push_back(0);
		arr.data.push_back(0);
		arr.data.push_back(0);
	}

	float num1, num2;

    if (ground_depth <= 1.0 || p.depth <= 1.
		|| p.depth > 4. || stop_hcsr || timer == 0) {
		//arr.data[0] = 0;
		//arr.data[1] = 0;

		num1 = 0;
		num2 = 0;
    }
	else {
		float center = p.x - 0.5;

		if (center < 0.15 && center > -0.15) {
			//arr.data[0] = 40;
			//arr.data[1] = 40;
		
			num1 = 30;
			num2 = 30;
		}
		else {
			//arr.data[0] = 40 - center * 10;
			//arr.data[1] = 40 + center * 10;

			num1 = 30 - center * 15;
			num2 = 30 + center * 15;
		}	
	}

	//float newspeed1 = relieveSpeed(arr.data[0], num1);
	//float newspeed2 = relieveSpeed(arr.data[1], num2);

	//arr.data[0] = newspeed1;
	//arr.data[1] = newspeed2;

	arr.data[0] = num1;
	arr.data[1] = num2;

	ROS_INFO("Depth: %f", p.depth);
	ROS_INFO("Send Motor1 = %f, Motor2 = %f", arr.data[0], arr.data[1]);
	td_pub_ij.publish(arr);
	//arr.data.clear();
}

int main(int argc, char **argv)
{
  ros::init(argc,argv,"motor_controller");
  ros::NodeHandle nh;
  //pthread_t th;

  //pthread_mutex_init(&mutex_timer, NULL);
  //pthread_mutex_init(&mutex_person, NULL);
  //pthread_mutex_init(&mutex_xy, NULL);
  stop_hcsr = false;
  stop = true;
  timer = 0;

  ros::Timer t = nh.createTimer(ros::Duration(0.1), move);

  td_pub_ij = nh.advertise<std_msgs::Float64MultiArray>("motor_theta_dist",40);
  ros::Subscriber subs_hcsr = nh.subscribe("hcsr", 1, hcsr_callback);
  ros::Subscriber subs_ai_rgb = nh.subscribe("ai", 1, ai_rgb_callback);
  ros::Subscriber subs_ai_depth = nh.subscribe("/camera/depth/image", 1, ai_depth_callback);

  ros::Rate rate(10);

  int cnt = 0;

  //struct Person p;

  while (ros::ok()) {

	//pthread_mutex_lock(&mutex_person);

	p.depth = 100.;
	for (int i = 0; i < persons.size(); i++) {
		if (p.depth > persons[i].depth) {
			p.x = persons[i].x;
			p.y = persons[i].y;
			p.depth = persons[i].depth;
		}
	}
	//pthread_mutex_unlock(&mutex_person);

	//move(td_pub_ij, p);

    cnt++;

    if (cnt == 5) {
		if (timer > 0) {      
			//pthread_mutex_lock(&mutex_timer);
  	 	 	timer -= 0.5;
      		//pthread_mutex_unlock(&mutex_timer);
		}
      	cnt = 0;
    }

    rate.sleep();
    ros::spinOnce();
  }

  return 0;

}
