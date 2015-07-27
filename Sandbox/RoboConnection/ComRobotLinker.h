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
	bool ReadPacket(MavlinkPacket* packet);

    virtual void OpenPort(QString name);
	virtual void GetParamList();

	void SetControlSystem(ArducopterControlSystem* system);
private:
	ArducopterControlSystem* m_controlSystem;
    ComConnection* copterCom;
};