#include "JoysticThreads.h"



JoystickThread::JoystickThread(Joystick* joystick, JoystickToBufferController* buffer, ArducopterControlSystem* controlSystem){
    this->joystick = joystick;
    this->buffer = buffer;

	this->m_controlSystem = controlSystem;
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
		if (m_controlSystem->isRedButtonPressed())
		{
			std::cout << "red button activated\n";
			while (m_controlSystem->isRedButtonPressed())
			{
				this->msleep(1);
			}
			std::cout << "red button deactivated\n";
		}
        buffer->WriteJoystickData(&data); 
        QThread::currentThread()->msleep(buffer->MsToWait());
    }
}


RobotLinkThread::RobotLinkThread(DataOutputController* buffer, RobotLinker* link, MavlinkVisitor* visitor, ArducopterControlSystem* controlSystem)
{
    this->m_buffer = buffer;
    this->m_link = link;
    this->m_visitor = visitor;
	this->m_controlSystem = controlSystem;

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
		if (m_controlSystem->isRedButtonPressed())
		{
			Stop();
			std::cout << "sending packets stopped\n";
			std::cout << "sended: " << m_count << std::endl;
			
			while (m_controlSystem->isRedButtonPressed())
			{
				this->msleep(1);
			}
		}
			
		else
		{
			m_buffer->Read(&m_packet, m_visitor);  //получение данных, формирование  Mavlink-пакета
		
			m_link->SendPacket(&m_packet); //отправка Mavlink-пакета
			m_count++;
		}

        this->msleep(1);
    }
}

void RobotLinkThread::sleep_m(int millis){
    this->msleep(millis);
}

RobotLinkerReadingThread::RobotLinkerReadingThread(RobotLinker* linker, DataInputController* controller)
{
	m_inputController = controller;
	m_robotLinker = linker;
	m_packet = new MavlinkPacket();
}

void RobotLinkerReadingThread::run()
{
	while (true){
		m_robotLinker->ReadPacket(m_packet);
		m_inputController->WriteMavlinkPacket(m_packet);
	}
}

CameraDbgThread::CameraDbgThread(DataHandler<CrossPoint2D> *dh_out, TSDataHandler<Mat> *dbg_outputImage)
{
	m_DataHandler_out = dh_out;
	m_dbg_outputImage = dbg_outputImage;
}
void CameraDbgThread::run()
{
	while (true)
	{
		CrossPoint2D out;
		if (m_DataHandler_out->Peek(out) && m_dbg_outputImage->Read(img))
		{
			//cout << "[" << out.GetX() << ';' << out.GetY() << "]" <<  endl;
			imshow("OUTPUT", img);
			waitKey(1);
		}
	}
}