#include "ros/ros.h"

#include "std_msgs/Float64MultiArray.h" //theta, distance
#include "std_msgs/String.h"            //done
#include "std_msgs/Float64.h"

bool flag=1;                            //flag
ros::Publisher td_pub_ij;

//done subscribe
void d_msgCallback(const std_msgs::String::ConstPtr& if_done)
{
 ROS_INFO("recieved : %s",if_done->data.c_str());
 flag=1;
}

void m_msgCallback(const std_msgs::Float64::ConstPtr& data)
{
  std_msgs::Float64MultiArray arr;

  float num1 = 0.f;
  float num2 = 0.f;

  float center = data->data - 0.5f;

  num1 = 50.f * (1.f - center);
  num2 = 50.f * (1.f + center);

  arr.data.push_back(num1);
  arr.data.push_back(num2);

  ROS_INFO("Send Motor1 = %f, Motor2 = %f", num1, num2);
  td_pub_ij.publish(arr);

  arr.data.clear();
}

int main(int argc, char **argv)
{
  ros::init(argc,argv,"pub_theta_dist");
  ros::NodeHandle nh;

  td_pub_ij=nh.advertise<std_msgs::Float64MultiArray>("motor_theta_dist",1);
  ros::Subscriber td_sub_it=nh.subscribe("angle_control_done",1,d_msgCallback);
  ros::Subscriber subs = nh.subscribe("ai", 1, m_msgCallback);
  
  ros::spin();
  return 0;

}
