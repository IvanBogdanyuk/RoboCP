#pragma once

#include "joystickData.h"
#include "robotLink.h"
#include "com_connection.h"
#include <robo_mavlink_test\mavlink.h>

bool com_checksum(uint8_t *data, int32_t length, uint8_t com_ck_a, uint8_t com_ck_b); //Gets an array, length of its payload 
//and checksums on the com-ports

bool com_checksum(uint8_t *data, int32_t length); //Gets an array, length of its payload 

class ComRobotLinker : public RobotLinker{
private:
	ComConnection* copterCom;
public:

	ComRobotLinker();

	virtual void sendPacket(MavlinkPacket* packet);
	virtual void openPort(QString name);
};