#pragma once
#include "joystickData.h"

class RobotLinker{
public:
	virtual void sendPacket(MavlinkPacket* packet) = 0;
};