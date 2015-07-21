#pragma once

#include "joystickData.h"
#include "robotLink.h"
#include "com_connection.h"
#include <robo_mavlink_test\mavlink.h>



class ComRobotLinker : public RobotLinker
{

public:
    ComRobotLinker();
    virtual void SendPacket(MavlinkPacket* packet);
    virtual void OpenPort(QString name);
	virtual void GetParamList();
private:
    ComConnection* copterCom;
};