#include "mavlink_vis.h"

ComMavlinkVisitor::ComMavlinkVisitor(){
	mavlink_system.sysid = 255;    //ID of sending system
	mavlink_system.compid = 190;	//Component of sending system
	mavlink_target.sysid = 1;      //ID of target system
	mavlink_target.compid = 1;		//Component of target system


	system_type = 6;				 //MAV_TYPE ok
	autopilot_type = 8;              //MAV_AUTOPILOT ok (MAV_AUTOPILOT_INVALID)
	system_mode = 0;                 //MAV_MODE_PREFLIGHT ok (MAV_MODE_PREFLIGHT)
	custom_mode = 0;				 //For MAV_MODE_FLAG ok     
	system_state = 3;                //MAV_STATE ok (MAV_STATE_STANDBY)
}
void ComMavlinkVisitor::visitHeartBeat(MavlinkPacket* result)
{
	mavlink_message_t msg; //Struct for a mavlink message
	                       //Packing the message
	mavlink_msg_heartbeat_pack(mavlink_system.sysid, mavlink_system.compid, &msg, system_type, autopilot_type, system_mode, custom_mode, system_state);
	                       //Sending the message
	result->len = mavlink_msg_to_send_buffer(result->data, &msg);
}
void ComMavlinkVisitor::visitRc_Channels_Override(MavlinkPacket* result, unsigned short pitch, unsigned short roll, unsigned short gas, unsigned short rudder)
{
	mavlink_message_t msg; //Struct for a mavlink message
	                       //Packing the message
	mavlink_msg_rc_channels_override_pack(mavlink_system.sysid, mavlink_system.compid, &msg, mavlink_target.sysid, mavlink_target.compid, pitch, roll, gas, rudder, NULL, NULL, NULL, NULL);
	                       //Sending the message
	result->len = mavlink_msg_to_send_buffer(result->data, &msg);
}