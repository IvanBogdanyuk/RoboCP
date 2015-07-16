#pragma once

#include "joystickData.h"
#include "robotLink.h"
#include "com_connection.h"
#include <robo_mavlink_test\mavlink.h>

bool com_checksum(uint8_t *data, int32_t length); //Gets an array, length of its payload 

class ComRobotLinker : public RobotLinker{

public:
	ComRobotLinker();
	virtual void sendPacket(MavlinkPacket* packet);
	virtual void openPort(QString name);
private:
	ComConnection* copterCom;
};