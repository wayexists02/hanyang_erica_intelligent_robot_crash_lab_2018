#include "arms/Arms.h"
#include "arms/arms_node.h"
#include "ros/ros.h"
#include "std_srvs/Trigger.h"
#include "dynamixel_sdk/dynamixel_sdk.h"
#include <cmath>

void Arms::init()
{
	portHandler = dynamixel::PortHandler::getPortHandler(DEVICE_NAME);
	packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL);

	right_sub = handle.subscribe("right_arm_oper", 1, right_callback);
	left_sub = handle.subscribe("left_arm_oper", 1, left_callback);
	//check_server = handle.advertiseService("arms_done", check_callback);
	check_client = handle.serviceClient<std_srvs::Trigger>("arms_done");
	timer = handle.createTimer(ros::Duration(0.1), timer_callback);

	if (portHandler->openPort() == 0) {
		ROS_ERROR("Failed to open port of DXL");
		return;
	}

	if (portHandler->setBaudRate(BAUD_RATE) == 0) {
		ROS_ERROR("Failed to change the baud rate");
		return;
	}

	uint8_t error;

	packetHandler->write1ByteTxRx(
		portHandler,
		RIGHT_SHOULDER,
		ADDR_TORQUE,
		TORQUE_ENABLE,
		&error
	);

	packetHandler->write1ByteTxRx(
		portHandler,
		RIGHT_UPPER,
		ADDR_TORQUE,
		TORQUE_ENABLE,
		&error
	);

	packetHandler->write1ByteTxRx(
		portHandler,
		RIGHT_LOWER,
		ADDR_TORQUE,
		TORQUE_ENABLE,
		&error
	);

	packetHandler->write1ByteTxRx(
		portHandler,
		LEFT_SHOULDER,
		ADDR_TORQUE,
		TORQUE_ENABLE,
		&error
	);

	packetHandler->write1ByteTxRx(
		portHandler,
		LEFT_UPPER,
		ADDR_TORQUE,
		TORQUE_ENABLE,
		&error
	);

	packetHandler->write1ByteTxRx(
		portHandler,
		LEFT_LOWER,
		ADDR_TORQUE,
		TORQUE_ENABLE,
		&error
	);

	packetHandler->write2ByteTxRx(
		portHandler,
		RIGHT_SHOULDER,
		ADDR_MOVING_SPEED,
		(uint16_t)120,
		&error
	);

	packetHandler->write2ByteTxRx(
		portHandler,
		RIGHT_UPPER,
		ADDR_MOVING_SPEED,
		(uint16_t)120,
		&error
	);

	packetHandler->write2ByteTxRx(
		portHandler,
		RIGHT_LOWER,
		ADDR_MOVING_SPEED,
		(uint16_t)120,
		&error
	);

	packetHandler->write2ByteTxRx(
		portHandler,
		LEFT_SHOULDER,
		ADDR_MOVING_SPEED,
		(uint16_t)120,
		&error
	);

	packetHandler->write2ByteTxRx(
		portHandler,
		LEFT_UPPER,
		ADDR_MOVING_SPEED,
		(uint16_t)120,
		&error
	);

	packetHandler->write2ByteTxRx(
		portHandler,
		LEFT_LOWER,
		ADDR_MOVING_SPEED,
		(uint16_t)120,
		&error
	);
}

void Arms::setRightGoalPosition(uint16_t shoulder, uint16_t upper, uint16_t lower)
{
	this->right_shoulder = shoulder;
	this->right_upper = upper;
	this->right_lower = lower;

	uint8_t error;

	packetHandler->write2ByteTxRx(
		portHandler,
		RIGHT_SHOULDER,
		ADDR_GOAL_POSITION,
		shoulder,
		&error
	);	

	packetHandler->write2ByteTxRx(
		portHandler,
		RIGHT_UPPER,
		ADDR_GOAL_POSITION,
		upper,
		&error
	);

	packetHandler->write2ByteTxRx(
		portHandler,
		RIGHT_LOWER,
		ADDR_GOAL_POSITION,
		lower,
		&error
	);

}

void Arms::setLeftGoalPosition(uint16_t shoulder, uint16_t upper, uint16_t lower)
{
	this->left_shoulder = shoulder;
	this->left_upper = upper;
	this->left_lower = lower;

	uint8_t error;

	packetHandler->write2ByteTxRx(
		portHandler,
		LEFT_SHOULDER,
		ADDR_GOAL_POSITION,
		shoulder,
		&error
	);	

	packetHandler->write2ByteTxRx(
		portHandler,
		LEFT_UPPER,
		ADDR_GOAL_POSITION,
		upper,
		&error
	);

	packetHandler->write2ByteTxRx(
		portHandler,
		LEFT_LOWER,
		ADDR_GOAL_POSITION,
		lower,
		&error
	);
}

void Arms::trigger()
{
	std_srvs::Trigger trigger;
	check_client.call(trigger);
}

bool Arms::checkForFinish()
{
	uint8_t error;
	uint16_t present_right_shoulder, present_right_upper, present_right_lower;
	uint16_t present_left_shoulder, present_left_upper, present_left_lower;

	packetHandler->read2ByteTxRx(
		portHandler,
		RIGHT_SHOULDER,
		ADDR_PRESENT_POSITION,
		&present_right_shoulder,
		&error
	);

	packetHandler->read2ByteTxRx(
		portHandler,
		RIGHT_UPPER,
		ADDR_PRESENT_POSITION,
		&present_right_upper,
		&error
	);

	packetHandler->read2ByteTxRx(
		portHandler,
		RIGHT_LOWER,
		ADDR_PRESENT_POSITION,
		&present_right_lower,
		&error
	);

	packetHandler->read2ByteTxRx(
		portHandler,
		LEFT_SHOULDER,
		ADDR_PRESENT_POSITION,
		&present_left_shoulder,
		&error
	);

	packetHandler->read2ByteTxRx(
		portHandler,
		LEFT_UPPER,
		ADDR_PRESENT_POSITION,
		&present_left_upper,
		&error
	);

	packetHandler->read2ByteTxRx(
		portHandler,
		LEFT_LOWER,
		ADDR_PRESENT_POSITION,
		&present_left_lower,
		&error
	);

	if (std::abs(present_right_shoulder - right_shoulder) < 5 &&
	    std::abs(present_right_upper - right_upper) < 5 &&
	    std::abs(present_right_lower - right_lower) < 5 &&
	    std::abs(present_left_shoulder - left_shoulder) < 5 &&
	    std::abs(present_left_upper - left_upper) < 5 &&
	    std::abs(present_left_lower - left_lower) < 5)
		return true;
	else
		return false;
}
