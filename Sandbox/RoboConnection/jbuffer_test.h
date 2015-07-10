#pragma once
#include "joystickData.h"
#include "robotLink.h"
#include<ctime>
#include <SDL.h>

//mock implementations of a joysick and com-port connection
/*
class MockJoystick : public Joystick{
public:
	MockJoystick(){
		srand(time(NULL));
	}
	virtual void getJoysticState(JoystickData* data){
		data->rudder = rand();
		data->gas = rand();
		data->pitch = rand();
		data->roll = rand();
	}
};

class MockRobotLinker : public RobotLinker{
public:
	MockRobotLinker(){}
	virtual void sendPacket(MavlinkPacket* packet){
		packet->toString();

	};
};
*/