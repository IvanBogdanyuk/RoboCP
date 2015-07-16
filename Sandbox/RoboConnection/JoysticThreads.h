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
    JoystickThread(Joystick* joystick, MavlinkBuffer* buffer);
    void run();    
private:
    JoystickData data;
    Joystick* joystick;
    MavlinkBuffer* buffer;
};

class RobotLinkThread : public QThread{
public:
    RobotLinkThread(MavlinkBuffer* buffer, RobotLinker* link, MavlinkVisitor* visitor);
    void run();
    void sleep_m(int millis);
private:
    MavlinkBuffer* m_buffer;
    RobotLinker* m_link;
    MavlinkPacket m_packet;
    MavlinkVisitor* m_visitor;
};
#endif