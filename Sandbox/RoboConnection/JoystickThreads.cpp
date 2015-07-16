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
#ifdef PROFILING
        long timer = time(NULL);
#endif
        joystick->getJoysticState(&data);  //��������� ������
        int waitTime = buffer->writeJoystickData(&data); //��� ��� ������, ��� ������ ����� � ������

#ifdef PROFILING
        joystickTime += time(0) - timer;
        joystickTimes++;
#endif

        QThread::currentThread()->msleep(waitTime);
    }
}

RobotLinkThread::RobotLinkThread(MavlinkBuffer* buffer, RobotLinker* link, MavlinkVisitor* visitor){
    this->m_buffer = buffer;
    this->m_link = link;
    this->m_visitor = visitor;
}

void RobotLinkThread::run(){
    m_link->openPort("COM5"); 
    while (true)
    {
#ifdef PROFILING
        long timer = time(NULL);
#endif
        m_buffer->read(&m_packet, m_visitor);  //��������� ������ � �������� � ���� Mavlink-������
        m_link->sendPacket(&m_packet);
        
#ifdef PROFILING
        robotLinkTime += time(0) - timer;
        robotLinkTimes++;
#endif
        this->msleep(1);
    }
}

void RobotLinkThread::sleep_m(int millis){
    this->msleep(millis);
}

