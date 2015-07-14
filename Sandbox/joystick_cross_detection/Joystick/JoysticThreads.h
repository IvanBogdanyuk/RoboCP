#ifndef __JOYSTICK_THREADS_H__
#define __JOYSTICK_THREADS_H__

#include "joystickData.h"
#include "QtCore\qthread.h"
#include "QtCore\qstring.h"
#include "QtCore\qelapsedtimer.h"
#include "com_connection.h"
#include "robotLink.h"

#include "preprocess.h"

class JoystickThread : public QThread
{
	JoystickData data;
	Joystick* joystick;
	MavlinkBuffer* buffer;
public:
	JoystickThread(Joystick* joystick, MavlinkBuffer* buffer);
	
	void run();	
};

class RobotLinkThread : public QThread{
	MavlinkBuffer* buffer;
	RobotLinker* link;
	MavlinkPacket packet;
	MavlinkVisitor* visitor;
public:
	RobotLinkThread(MavlinkBuffer* buffer, RobotLinker* link, MavlinkVisitor* visitor);

	void run();

	void sleep_m(int millis);
};

#endif