#pragma once
#include "joystickData.h"
#include "QtCore\qthread.h"
#include "QtCore\qstring.h"
#include "qelapsedtimer.h"
#include "com_connection.h"

class JoystickThread : public QThread
{
	JoystickData data;
	Joystick* joystick;
	MavlinkBuffer* buffer;
	const int cJoystickRate = 59;
public:

	JoystickThread(Joystick* joystick, MavlinkBuffer* buffer){
		this->joystick = joystick;
		this->buffer = buffer;
	}

	// overriding the QThread's run() method
	void run()
	{

		while (true)
		{
			joystick->getJoysticState(&data);
			buffer->writeJoystickData(&data);
			//std::cout << "2" << std::endl;
		}

	}
	
	
};

class RobotLinkThread : public QThread{
	MavlinkBuffer* buffer;
	RobotLinker* link;
	MavlinkPacket packet;
	MavlinkVisitor* visitor;
public:
	RobotLinkThread(MavlinkBuffer* buffer, RobotLinker* link, MavlinkVisitor* visitor){
		this->buffer = buffer;
		this->link = link;
		this->visitor = visitor;
	}

	void run(){
		link->openPort("COM5");

		while (true)
		{
			buffer->read(&packet, visitor);
			link->sendPacket(&packet);
			//std::cout << "1"<<std::endl;
		}

	}

	void sleep_m(int millis){
		this->msleep(millis);
	}
};
/*
class ComListener :public QThread{
public:
	RobotLinker* link;
	QByteArray arr;
	ComListener(RobotLinker* link)
	{
		this->link = link;
	}
	void run()
	{
		
		while (true)
		{
			link->readPacket(arr);
			if (!arr.isEmpty())
				std::cout << "asas";
		}
	}
};
*/





