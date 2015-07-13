#include <iostream>
#include <vector>

#include "jbuffer_test.h"
#include "JoysticThreads.h"
#include "ComRobotLinker.h"

#include <QtCore\QCoreApplication>
#include <QtCore\qdebug.h>
 
#include <QtSerialPort\QSerialPort>
#include <QtSerialPort\QSerialPortInfo>
#include "mavlink_vis.h"

QT_USE_NAMESPACE

int main(int argc, char *argv[])
{

	//uncomment this to check the QtSerialPort example
	//return mainExample(argc, argv);

	Joystick* joystick = new MockJoystick();	//initializing a joystick
	MavlinkBuffer* buffer = new CircularJoystickBuffer(10);	
	RobotLinker* link = new MockRobotLinker();	//initializing a com-port connection
	MavlinkVisitor* mavlinkvisitor = new ComMavlinkVisitor();

	JoystickThread* jthread = new JoystickThread(joystick, buffer);	//thread that reads joysick state and pushes it to the buffer
	RobotLinkThread* rthread = new RobotLinkThread(buffer, link, mavlinkvisitor);	//thread that reads the latest joystick state to a buffer and sends it via com-port

	jthread->start();
	rthread->start();

	int i;
	std::cin >> i;
	return 0;
	
}
