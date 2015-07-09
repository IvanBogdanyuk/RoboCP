#pragma once
#include "joystickData.h"
#include "QtCore\qthread.h"
#include "QtCore\qstring.h"


class JoystickThread : public QThread
{
	JoystickData data;
	Joystick* joystick;
	MavlinkBuffer* buffer;
public:
	
	JoystickThread(Joystick* joystick, MavlinkBuffer* buffer){
		this->joystick = joystick;
		this->buffer = buffer;
	}

	// overriding the QThread's run() method
	void run(){
		while(true){
			joystick->getJoysticState(&data);
			buffer->writeJoystickData(&data);
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
		while(true){
			buffer->read(&packet, visitor);
			link->sendPacket(&packet);
		}
	}
	
	void sleep_m(int millis){
		this->msleep(millis);
	}
};






