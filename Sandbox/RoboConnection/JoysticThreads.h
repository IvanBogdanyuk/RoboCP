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
public:
    JoystickThread(Joystick* joystick, DataInputController* buffer);
    void run();    
private:
    JoystickData data;
    Joystick* joystick;
    DataInputController* buffer;
};

class RobotLinkThread : public QThread{
public:
    RobotLinkThread(DataOutputController* buffer, RobotLinker* link, MavlinkVisitor* visitor, Joystick* joystick);
    void run();
    void sleep_m(int millis);
private:
    DataOutputController* m_buffer;
    RobotLinker* m_link;
    MavlinkPacket m_packet;
    MavlinkVisitor* m_visitor;
	Joystick* m_joystick;

	int m_count;
	void Stop();
};
#endif