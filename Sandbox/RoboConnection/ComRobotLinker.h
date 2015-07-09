#pragma once

#include "joystickData.h"
#include "robotLink.h"
#include "com_connection.h"

class ComRobotLinker : public RobotLinker{
private:
	unsigned char DataToSend[32];
	ComConnection* copterCom;
public:

	ComRobotLinker(){
		copterCom = new ComConnection();
		sendInitionalData();
	}

	virtual void sendPacket(MavlinkPacket* packet){
		int length = -1;

		//TODO: deciding on what length of the packet is
		
		copterCom->Write((char*) packet->data, length);
	}

	void sendInitionalData(void){
		#ifdef ENABLE_LOGGING
		RAW_LOG(INFO, "ArduCopterController: sending initional data...");
		#endif
		unsigned char DataToSend[32];
		DataToSend[0] = 0xFE;
		DataToSend[1] = 0x02;
		DataToSend[2] = 0x00;
		DataToSend[3] = 0xFF;
		DataToSend[4] = 0xBE;
		DataToSend[5] = 0x15;
		DataToSend[6] = 0x01;
		DataToSend[7] = 0x01;
		DataToSend[8] = 0x79;
		DataToSend[9] = 0x37;
		copterCom->Write((char *)&DataToSend[0],10);
		((RobotLinkThread*) QThread::currentThread())->sleep_m(1000);
  
		DataToSend[0] = 0xFE;
		DataToSend[1] = 0x09;
		DataToSend[2] = 0x01;
		DataToSend[3] = 0xFF;
		DataToSend[4] = 0xBE;
		DataToSend[5] = 0x00;
		DataToSend[6] = 0x00;
		DataToSend[7] = 0x00;
		DataToSend[8] = 0x00;
		DataToSend[9] = 0x00;
		DataToSend[10] = 0x06;
		DataToSend[11] = 0x08;
		DataToSend[12] = 0x00;
		DataToSend[12] = 0x00;
		DataToSend[13] = 0x03;
		DataToSend[14] = 0xC2;
		DataToSend[15] = 0x3C;
		copterCom->Write((char *)&DataToSend[0],16);
		((RobotLinkThread*) QThread::currentThread())->sleep_m(50);

		DataToSend[0] = 0xFE; 
		DataToSend[1] = 0x06;
		DataToSend[2] = 0x02;
		DataToSend[3] = 0xFF;
		DataToSend[4] = 0xBE;
		DataToSend[5] = 0x42;
		DataToSend[6] = 0x02;
		DataToSend[7] = 0x00;
		DataToSend[8] = 0x01;
		DataToSend[9] = 0x01;
		DataToSend[10] = 0x02;
		DataToSend[11] = 0x01;
		DataToSend[12] = 0xC0;
		DataToSend[13] = 0x34;
		copterCom->Write((char *)&DataToSend[0],14);
		((RobotLinkThread*) QThread::currentThread())->sleep_m(100);
  
		DataToSend[0] = 0xFE; 
		DataToSend[1] = 0x06;
		DataToSend[2] = 0x03;
		DataToSend[3] = 0xFF;
		DataToSend[4] = 0xBE;
		DataToSend[5] = 0x42;
		DataToSend[6] = 0x02;
		DataToSend[7] = 0x00;
		DataToSend[8] = 0x01;
		DataToSend[9] = 0x01;
		DataToSend[10] = 0x02;
		DataToSend[11] = 0x01;
		DataToSend[12] = 0x51;
		DataToSend[13] = 0x61;
		copterCom->Write((char *)&DataToSend[0],14);
		((RobotLinkThread*) QThread::currentThread())->sleep_m(50);

		DataToSend[0] = 0xFE; 
		DataToSend[1] = 0x06;
		DataToSend[2] = 0x04;
		DataToSend[3] = 0xFF;
		DataToSend[4] = 0xBE;
		DataToSend[5] = 0x42;
		DataToSend[6] = 0x03;
		DataToSend[7] = 0x00;
		DataToSend[8] = 0x01;
		DataToSend[9] = 0x01;
		DataToSend[10] = 0x06;
		DataToSend[11] = 0x01;
		DataToSend[12] = 0x03;
		DataToSend[13] = 0x3D;
		copterCom->Write((char *)&DataToSend[0],14);
		((RobotLinkThread*) QThread::currentThread())->sleep_m(50);

		DataToSend[0] = 0xFE; 
		DataToSend[1] = 0x06;
		DataToSend[2] = 0x05;
		DataToSend[3] = 0xFF;
		DataToSend[4] = 0xBE;
		DataToSend[5] = 0x42;
		DataToSend[6] = 0x03;
		DataToSend[7] = 0x00;
		DataToSend[8] = 0x01;
		DataToSend[9] = 0x01;
		DataToSend[10] = 0x06;
		DataToSend[11] = 0x01;
		DataToSend[12] = 0x92;
		DataToSend[13] = 0x68;
		copterCom->Write((char *)&DataToSend[0],14);
		((RobotLinkThread*) QThread::currentThread())->sleep_m(50);

		DataToSend[0] = 0xFE; 
		DataToSend[1] = 0x06;
		DataToSend[2] = 0x06;
		DataToSend[3] = 0xFF;
		DataToSend[4] = 0xBE;
		DataToSend[5] = 0x42;
		DataToSend[6] = 0x0A;
		DataToSend[7] = 0x00;
		DataToSend[8] = 0x01;
		DataToSend[9] = 0x01;
		DataToSend[10] = 0x0A;
		DataToSend[11] = 0x01;
		DataToSend[12] = 0xBB;
		DataToSend[13] = 0x72;
		copterCom->Write((char *)&DataToSend[0],14);
		((RobotLinkThread*) QThread::currentThread())->sleep_m(50);

		DataToSend[0] = 0xFE; 
		DataToSend[1] = 0x06;
		DataToSend[2] = 0x07;
		DataToSend[3] = 0xFF;
		DataToSend[4] = 0xBE;
		DataToSend[5] = 0x42;
		DataToSend[6] = 0x0A;
		DataToSend[7] = 0x00;
		DataToSend[8] = 0x01;
		DataToSend[9] = 0x01;
		DataToSend[10] = 0x0A;
		DataToSend[11] = 0x01;
		DataToSend[12] = 0x2A;
		DataToSend[13] = 0x27;
		copterCom->Write((char *)&DataToSend[0],14);
		((RobotLinkThread*) QThread::currentThread())->sleep_m(50);

		DataToSend[0] = 0xFE; 
		DataToSend[1] = 0x06;
		DataToSend[2] = 0x08;
		DataToSend[3] = 0xFF;
		DataToSend[4] = 0xBE;
		DataToSend[5] = 0x42;
		DataToSend[6] = 0x0A;
		DataToSend[7] = 0x00;
		DataToSend[8] = 0x01;
		DataToSend[9] = 0x01;
		DataToSend[10] = 0x0B;
		DataToSend[11] = 0x01;
		DataToSend[12] = 0xBA;
		DataToSend[13] = 0x61;
		copterCom->Write((char *)&DataToSend[0],14);
		((RobotLinkThread*) QThread::currentThread())->sleep_m(50);

		DataToSend[0] = 0xFE; 
		DataToSend[1] = 0x06;
		DataToSend[2] = 0x09;
		DataToSend[3] = 0xFF;
		DataToSend[4] = 0xBE;
		DataToSend[5] = 0x42;
		DataToSend[6] = 0x0A;
		DataToSend[7] = 0x00;
		DataToSend[8] = 0x01;
		DataToSend[9] = 0x01;
		DataToSend[10] = 0x0B;
		DataToSend[11] = 0x01;
		DataToSend[12] = 0x2B;
		DataToSend[13] = 0x34;
		copterCom->Write((char *)&DataToSend[0],14);
		((RobotLinkThread*) QThread::currentThread())->sleep_m(100);

		DataToSend[0] = 0xFE; 
		DataToSend[1] = 0x06;
		DataToSend[2] = 0x0A;
		DataToSend[3] = 0xFF;
		DataToSend[4] = 0xBE;
		DataToSend[5] = 0x42;
		DataToSend[6] = 0x02;
		DataToSend[7] = 0x00;
		DataToSend[8] = 0x01;
		DataToSend[9] = 0x01;
		DataToSend[10] = 0x0C;
		DataToSend[11] = 0x01;
		DataToSend[12] = 0x71;
		DataToSend[13] = 0x98;
		copterCom->Write((char *)&DataToSend[0],14);
		((RobotLinkThread*) QThread::currentThread())->sleep_m(50);

		DataToSend[0] = 0xFE; 
		DataToSend[1] = 0x06;
		DataToSend[2] = 0x0B;
		DataToSend[3] = 0xFF;
		DataToSend[4] = 0xBE;
		DataToSend[5] = 0x42;
		DataToSend[6] = 0x02;
		DataToSend[7] = 0x00;
		DataToSend[8] = 0x01;
		DataToSend[9] = 0x01;
		DataToSend[10] = 0x0C;
		DataToSend[11] = 0x01;
		DataToSend[12] = 0xE0;
		DataToSend[13] = 0xCD;
		copterCom->Write((char *)&DataToSend[0],14);
		((RobotLinkThread*) QThread::currentThread())->sleep_m(50);

		DataToSend[0] = 0xFE; 
		DataToSend[1] = 0x06;
		DataToSend[2] = 0x0C;
		DataToSend[3] = 0xFF;
		DataToSend[4] = 0xBE;
		DataToSend[5] = 0x42;
		DataToSend[6] = 0x02;
		DataToSend[7] = 0x00;
		DataToSend[8] = 0x01;
		DataToSend[9] = 0x01;
		DataToSend[10] = 0x01;
		DataToSend[11] = 0x01;
		DataToSend[12] = 0x79;
		DataToSend[13] = 0x92;
		copterCom->Write((char *)&DataToSend[0],14);
		((RobotLinkThread*) QThread::currentThread())->sleep_m(50);

		DataToSend[0] = 0xFE; 
		DataToSend[1] = 0x06;
		DataToSend[2] = 0x0D;
		DataToSend[3] = 0xFF;
		DataToSend[4] = 0xBE;
		DataToSend[5] = 0x42;
		DataToSend[6] = 0x02;
		DataToSend[7] = 0x00;
		DataToSend[8] = 0x01;
		DataToSend[9] = 0x01;
		DataToSend[10] = 0x01;
		DataToSend[11] = 0x01;
		DataToSend[12] = 0xE8;
		DataToSend[13] = 0xC7;
		copterCom->Write((char *)&DataToSend[0],14);
		((RobotLinkThread*) QThread::currentThread())->sleep_m(50);

		DataToSend[0] = 0xFE; 
		DataToSend[1] = 0x06;
		DataToSend[2] = 0x0E;
		DataToSend[3] = 0xFF;
		DataToSend[4] = 0xBE;
		DataToSend[5] = 0x42;
		DataToSend[6] = 0x02;
		DataToSend[7] = 0x00;
		DataToSend[8] = 0x01;
		DataToSend[9] = 0x01;
		DataToSend[10] = 0x03;
		DataToSend[11] = 0x01;
		DataToSend[12] = 0xE3;
		DataToSend[13] = 0x8C;
		copterCom->Write((char *)&DataToSend[0],14);
		((RobotLinkThread*) QThread::currentThread())->sleep_m(50);

		DataToSend[0] = 0xFE; 
		DataToSend[1] = 0x06;
		DataToSend[2] = 0x0F;
		DataToSend[3] = 0xFF;
		DataToSend[4] = 0xBE;
		DataToSend[5] = 0x42;
		DataToSend[6] = 0x02;
		DataToSend[7] = 0x00;
		DataToSend[8] = 0x01;
		DataToSend[9] = 0x01;
		DataToSend[10] = 0x03;
		DataToSend[11] = 0x01;
		DataToSend[12] = 0x72;
		DataToSend[13] = 0xD9;
		copterCom->Write((char *)&DataToSend[0],14);
		((RobotLinkThread*) QThread::currentThread())->sleep_m(50);

		#ifdef ENABLE_LOGGING
		RAW_LOG(INFO, "ArduCopterController: Initional data sent!");
		#endif
	}
};