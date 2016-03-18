#pragma once
#include "joystickData.h"
#include "QtCore\qthread.h"
#include "QtCore\qstring.h"


class JoystickThread : public QThread
{
	JoystickData data;
	Joystick* joystick;
	JoystickBuffer* buffer;
public:
	
	JoystickThread(Joystick* joystick, JoystickBuffer* buffer){
		this->joystick = joystick;
		this->buffer = buffer;
	}

	// overriding the QThread's run() method
	void run(){
		while(true){
			joystick->getJoysticState(&data);
			buffer->write(&data);
		}
	}
};

class RobotLinkThread : public QThread{
	JoystickBuffer* buffer;
	RobotLinker* link;
	JoystickData data;
public:
	RobotLinkThread(JoystickBuffer* buffer, RobotLinker* link){
		this->buffer = buffer;
		this->link = link;
	}

	void run(){
		while(true){
			buffer->read(&data);
			link->sendJoystickData(&data);
		}
	}
};






