#include "JoysticThreads.h"


JoystickThread::JoystickThread(Joystick* joystick, MavlinkBuffer* buffer){
	this->joystick = joystick;
	this->buffer = buffer;
}

// overriding the QThread's run() method
void JoystickThread::run()
{

	while (true)
	{
		joystick->getJoysticState(&data);
		QThread::currentThread()->msleep(buffer->writeJoystickData(&data));
	}

}

RobotLinkThread::RobotLinkThread(MavlinkBuffer* buffer, RobotLinker* link, MavlinkVisitor* visitor){
	this->buffer = buffer;
	this->link = link;
	this->visitor = visitor;
}

void RobotLinkThread::run(){
	link->openPort("COM5");

	while (true)
	{
		buffer->read(&packet, visitor);
		link->sendPacket(&packet);
		//std::cout << "1"<<std::endl;
	}

}

void RobotLinkThread::sleep_m(int millis){
	this->msleep(millis);
}

