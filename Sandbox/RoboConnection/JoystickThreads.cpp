#include "JoysticThreads.h"



JoystickThread::JoystickThread(Joystick* joystick, DataInputController* buffer){
    this->joystick = joystick;
    this->buffer = buffer;
}

// overriding the QThread's run() method
void JoystickThread::run()
{
    while (true)
    {
        joystick->GetJoysticState(&data);  //получение данных
		if (joystick->isDanger())
			break;
        buffer->WriteJoystickData(&data); //ждём тем дольше, чем меньше места в буфере

        QThread::currentThread()->msleep(1);
    }
}


RobotLinkThread::RobotLinkThread(DataOutputController* buffer, RobotLinker* link, MavlinkVisitor* visitor, Joystick* joystick){
    this->m_buffer = buffer;
    this->m_link = link;
    this->m_visitor = visitor;
	this->m_joystick = joystick;
}

void RobotLinkThread::Stop()
{
	JoystickData* jData = new JoystickData(1500, 1000, 1500, 1500);
	jData->ToMavlinkPacket(&m_packet, m_visitor);
	m_link->SendPacket(&m_packet);
}

void RobotLinkThread::run(){
    m_link->OpenPort("COM5"); 
    while (true)
    {
#ifdef PROFILING
        long timer = time(NULL);
#endif
		if (m_joystick->isDanger())
		{
			Stop();
			break;
		}
			
		else
		{
			m_buffer->Read(&m_packet, m_visitor);  //получение данных, формирование  Mavlink-пакета
		}
		if (m_joystick->isDanger())
		{
			Stop();
			break;
		}
		else
			m_link->SendPacket(&m_packet); //отправка Mavlink-пакета
        
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

