#pragma once
#include "joystickData.h"

class RobotLinker{
public:
	virtual void sendJoystickData(JoystickData* data) = 0;
};