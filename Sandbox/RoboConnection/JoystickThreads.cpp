#include "JoysticThreads.h"



JoystickThread::JoystickThread(Joystick* joystick, DataInputController* buffer){
    this->joystick = joystick;
    this->buffer = buffer;
}

// overriding the QThread's run() method
void JoystickThread::run()
{
	/*while (!joystick->hasBegun())
	{
		sleep(1);
		joystick->GetJoysticState(&data);
	}
	std::cout << "started to send joystick data\n\n\n\n\n\n\n";	*/
    while (true)
    {
        joystick->GetJoysticState(&data);  //получение данных
		if (joystick->isDanger())
		{
			std::cout << "red button activated\n";
			break;
		}
        buffer->WriteJoystickData(&data); 
        QThread::currentThread()->msleep(buffer->MsToWait());
    }
}


RobotLinkThread::RobotLinkThread(DataOutputController* buffer, RobotLinker* link, MavlinkVisitor* visitor, Joystick* joystick){
    this->m_buffer = buffer;
    this->m_link = link;
    this->m_visitor = visitor;
	this->m_joystick = joystick;

	m_count = 0;
}

void RobotLinkThread::Stop()
{
	JoystickData* jData = new JoystickData(1500, 1000, 1500, 1500);	//gas on zero
	jData->ToMavlinkPacket(&m_packet, m_visitor);
	m_link->SendPacket(&m_packet);
}

void RobotLinkThread::run(){
    m_link->OpenPort("COM3"); 

    while (true)
    {
		if (m_joystick->isDanger())
		{
			Stop();
			std::cout << "sending packets stopped\n";
			std::cout << "sended: " << m_count << std::endl;
			break;
		}
			
		else
		{
			m_buffer->Read(&m_packet, m_visitor);  //получение данных, формирование  Mavlink-пакета
		}
		if (m_joystick->isDanger())
		{
			Stop();
			std::cout << "sending packets stopped\n";
			std::cout << "sended: " << m_count << std::endl;
			break;
		}
		else
		{
			m_link->SendPacket(&m_packet); //отправка Mavlink-пакета
			m_count++;
		}

        this->msleep(1);
    }
}

void RobotLinkThread::sleep_m(int millis){
    this->msleep(millis);
}

