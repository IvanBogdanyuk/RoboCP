#pragma once
#include "joystickData.h"

class RobotLinker
{
public:
    virtual void SendPacket(MavlinkPacket* packet) = 0;
    virtual void OpenPort(QString name) = 0;
	virtual void GetParamList() = 0;
};