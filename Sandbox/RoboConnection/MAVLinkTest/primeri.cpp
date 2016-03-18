#include <iostream>
#include <common\mavlink.h>

using namespace std;

void printBuffer(uint8_t* buff, int length){
	for(int i = 0; i<length; i++){
		std::cout<< (unsigned int) buff[i]<<endl;
	}
}

//Спецификация MAVLink: https://pixhawk.ethz.ch/mavlink/
int main()
{
	mavlink_system_t mavlink_system;
	mavlink_system_t mavlink_target;

	mavlink_system.sysid  = 255;    //ID отправляющей системы
	mavlink_system.compid = 190;	//Компонент отправляющей системы

	mavlink_target.sysid  = 1;      //ID принимающей системы
	mavlink_target.compid = 1;		//Компонент принимающей системы

	uint8_t system_type = 6; //MAV_TYPE
	uint8_t autopilot_type = MAV_AUTOPILOT_INVALID; //MAV_AUTOPILOT ok

	uint8_t system_mode = MAV_MODE_PREFLIGHT; //MAV_MODE_PREFLIGHT ok
	uint32_t custom_mode = 0;                 //Для MAV_MODE_FLAG ok     
	uint8_t system_state = MAV_STATE_STANDBY; //MAV_STATE ok
	// Версия протокола MAVLink: 3

	mavlink_message_t msg;               // MAVLink сообщение
	uint8_t buf[MAVLINK_MAX_PACKET_LEN]; // Буфер

	uint16_t yaw = 1498;	//Тестирование функции, формирующей пакет rc_channels_override
	uint16_t pitch = 1501;  
	uint16_t roll = 1000;
	uint16_t gas = 1499;

	//mavlink_msg_heartbeat_pack(mavlink_system.sysid, mavlink_system.compid, &msg, system_type, autopilot_type, system_mode, custom_mode, system_state);
	mavlink_msg_rc_channels_override_pack(mavlink_system.sysid, mavlink_system.compid, &msg, mavlink_target.sysid,mavlink_target.compid, yaw, pitch, roll, gas, NULL, NULL, NULL, NULL);
	//mavlink_msg_param_request_list_pack(mavlink_system.sysid, mavlink_system.compid, &msg, mavlink_target.sysid,mavlink_target.compid);

	uint16_t len = mavlink_msg_to_send_buffer(buf, &msg); // Отправление пакета в буфер. Функция len возвращает длину пакета в байтах

	printBuffer(&buf[0], MAVLINK_MAX_PACKET_LEN); //Вывод буфера на поток cout
	getchar();
	return 0;
}