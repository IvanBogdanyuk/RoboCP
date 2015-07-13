#include "mavlink_vis.h"

ComMavlinkVisitor::ComMavlinkVisitor(){
	mavlink_system.sysid = 255;    //ID of sending system
	mavlink_system.compid = 190;	//Component of sending system
	mavlink_target.sysid = 1;      //ID of target system
	mavlink_target.compid = 1;		//Component of target system

	system_type = 6;						 //MAV_TYPE ok
	autopilot_type = MAV_AUTOPILOT_INVALID;  //MAV_AUTOPILOT ok
	system_mode = MAV_MODE_PREFLIGHT;		 //MAV_MODE_PREFLIGHT ok
	custom_mode = 0;						 //For MAV_MODE_FLAG ok     
	system_state = MAV_STATE_STANDBY;		 //MAV_STATE ok
}
void ComMavlinkVisitor::visitHeartBeat(MavlinkPacket* result)
{
	mavlink_message_t msg; //Struct for mavmessage
	mavlink_msg_heartbeat_pack(mavlink_system.sysid, mavlink_system.compid, &msg, system_type, autopilot_type, system_mode, custom_mode, system_state);

	result->len = mavlink_msg_to_send_buffer(result->data, &msg);
	std::cout << result->len << std::endl;
}
void ComMavlinkVisitor::visitRc_Channels_Override(MavlinkPacket* result, unsigned short pitch, unsigned short roll, unsigned short gas, unsigned short rudder)
{
	mavlink_message_t msg; //Struct for mavmessage
	mavlink_msg_rc_channels_override_pack(mavlink_system.sysid, mavlink_system.compid, &msg, mavlink_target.sysid, mavlink_target.compid, pitch, roll, gas, rudder, NULL, NULL, NULL, NULL);
	result->len = mavlink_msg_to_send_buffer(result->data, &msg);


}