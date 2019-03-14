#ifndef __ARMS_H__
#define __ARMS_H__

#include "ros/ros.h"
#include "dynamixel_sdk/dynamixel_sdk.h"

#define TORQUE_ENABLE 1
#define TORQUE_DISABLE 0

#define UINT8_T uint8_t
#define UINT16_T uint16_t

#define RIGHT_SHOULDER ((UINT8_T)1)
#define RIGHT_UPPER ((UINT8_T)2)
#define RIGHT_LOWER ((UINT8_T)3)
#define LEFT_SHOULDER ((UINT8_T)4)
#define LEFT_UPPER ((UINT8_T)5)
#define LEFT_LOWER ((UINT8_T)6)

#define DEVICE_NAME "/dev/ttyUSB0"
#define BAUD_RATE 1000000
#define PROTOCOL 1.0

#define ADDR_TORQUE ((UINT16_T)24)
#define ADDR_GOAL_POSITION ((UINT16_T)30)
#define ADDR_MOVING_SPEED ((UINT16_T)32)
#define ADDR_PRESENT_POSITION ((UINT16_T)36)

class Arms {
public:
	Arms() {}
	virtual ~Arms() {}

	void init();
	void setRightGoalPosition(uint16_t shoulder, uint16_t upper, uint16_t lower);
	void setLeftGoalPosition(uint16_t shoulder, uint16_t upper, uint16_t lower);
	bool checkForFinish();
	void trigger();

private:
	ros::NodeHandle handle;
	ros::Subscriber right_sub;
	ros::Subscriber left_sub;
	ros::Timer timer;
	//ros::ServiceServer check_server;
	ros::ServiceClient check_client;
	dynamixel::PortHandler* portHandler;
	dynamixel::PacketHandler* packetHandler;
	uint16_t right_shoulder, right_upper, right_lower;
	uint16_t left_shoulder, left_upper, left_lower;
};

#endif
