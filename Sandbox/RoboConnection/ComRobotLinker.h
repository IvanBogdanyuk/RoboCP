#pragma once

#include "joystickData.h"
#include "robotLink.h"
#include "com_connection.h"

class ComRobotLinker : public RobotLinker{
private:
	//unsigned char DataToSend[32];
	ComConnection* copterCom;
public:

	ComRobotLinker(){
		copterCom = new ComConnection();
	}

	virtual void sendPacket(MavlinkPacket* packet){
		

		//TODO: deciding on what length of the packet is
		
		copterCom->Write( packet->data, packet->len);
		QByteArray buffer=copterCom->readPacket();
		if (buffer.length() > 5){
			int idd = (unsigned char)buffer[5];
			std::cout << "got obj with id: " << idd << std::endl;
		}
	}
	virtual void openPort(QString name)
	{
		copterCom->openPort(name);
		copterCom->getParamList();
	}
	/*
	virtual void readPacket(QByteArray res)
	{
		
	}
	*/
	
	
};