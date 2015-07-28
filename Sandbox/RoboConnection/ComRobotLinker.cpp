#include "ComRobotLinker.h"
#include "com_checksum.h"

ComRobotLinker::ComRobotLinker(){
    copterCom = new ComConnection();
}

void ComRobotLinker::SetControlSystem(ArducopterControlSystem* system)
{
	m_controlSystem = system;
}

void ComRobotLinker::SendPacket(MavlinkPacket* packet){


    //TODO: deciding on what length of the packet is

    copterCom->Write(packet->data, packet->len);
    QByteArray buffer = copterCom->readPacket();
    if (buffer.length() > 5){
        int idd = (unsigned char)buffer[5];
        if ((idd == 22) && (com_checksum(packet->data, 9)))
            std::cout << "checked:" << std::endl;

        std::cout << "got obj with id: " << idd << std::endl;	
    }
}

bool ComRobotLinker::ReadPacket(MavlinkPacket* packet)
{
	QByteArray& byteArray = copterCom->readPacket();
	memcpy(packet->data, byteArray.data(), byteArray.size());

	//TODO: return false if the packet is empty
	return true;
}

void ComRobotLinker::GetParamList()
{
	mavlink_system_t mavlink_system;
	mavlink_system_t mavlink_target;
	uint8_t system_type = 6; //MAV_TYPE
	uint8_t autopilot_type = MAV_AUTOPILOT_INVALID; //MAV_AUTOPILOT ok

	uint8_t system_mode = MAV_MODE_PREFLIGHT; //MAV_MODE_PREFLIGHT ok
	uint32_t custom_mode = 0;                 // MAV_MODE_FLAG ok     
	uint8_t system_state = MAV_STATE_STANDBY; //MAV_STATE ok
	mavlink_system.sysid = 255;    //System ID 
	mavlink_system.compid = 190;    //Component ID

	mavlink_target.sysid = 1;      //System ID 
	mavlink_target.compid = 1;        //Component ID
	mavlink_message_t msg;               //MAVLink message
	uint8_t buf[MAVLINK_MAX_PACKET_LEN]; //Init MAVLink buffer
	mavlink_msg_param_request_list_pack(mavlink_system.sysid, mavlink_system.compid,
		&msg, mavlink_target.sysid, mavlink_target.compid); //Packing the message
	uint16_t len = mavlink_msg_to_send_buffer(buf, &msg); // Sending the message to buffer
	copterCom -> Write((unsigned char*)buf, len);

	int packets = 0;
	int param_count = 10;
	do
	{
		QByteArray buffer = copterCom->readPacket();
		packets++;
		if ((unsigned char)buffer[5] == 22) //mavlink param value
			std::cout << "param val";
		if ((unsigned char)buffer[5] == 253) //status text
			std::cout << "status val";
		int idd = (unsigned char)buffer[5];
		std::cout << "got obj with id: " << idd << std::endl;
	} while (packets<50);	
}

void ComRobotLinker::OpenPort(QString name)
{
    copterCom->openPort(name);
    GetParamList();
}
