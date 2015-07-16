#include "mavlink_vis.h"

ComMavlinkVisitor::ComMavlinkVisitor(){
    m_mavlink_system.sysid = 255;    //ID of sending system
    m_mavlink_system.compid = 190;    //Component of sending system
    m_mavlink_target.sysid = 1;      //ID of target system
    m_mavlink_target.compid = 1;        //Component of target system


    m_system_type = 6;                 //MAV_TYPE ok
    m_autopilot_type = 8;              //MAV_AUTOPILOT ok (MAV_AUTOPILOT_INVALID)
    m_system_mode = 0;                 //MAV_MODE_PREFLIGHT ok (MAV_MODE_PREFLIGHT)
    m_custom_mode = 0;                 //For MAV_MODE_FLAG ok     
    m_system_state = 3;                //MAV_STATE ok (MAV_STATE_STANDBY)
}
void ComMavlinkVisitor::VisitHeartBeat(MavlinkPacket* result)
{
    mavlink_message_t msg; //Struct for a mavlink message
                           //Packing the message
    mavlink_msg_heartbeat_pack(m_mavlink_system.sysid, m_mavlink_system.compid, &msg, m_system_type, m_autopilot_type, m_system_mode, m_custom_mode, m_system_state);
                           //Sending the message
    result->len = mavlink_msg_to_send_buffer(result->data, &msg);
}
void ComMavlinkVisitor::VisitRc_Channels_Override(MavlinkPacket* result, unsigned short pitch, unsigned short roll, unsigned short gas, unsigned short rudder)
{
    mavlink_message_t msg; //Struct for a mavlink message
                           //Packing the message
    mavlink_msg_rc_channels_override_pack(m_mavlink_system.sysid, m_mavlink_system.compid, &msg, m_mavlink_target.sysid, m_mavlink_target.compid, pitch, roll, gas, rudder, NULL, NULL, NULL, NULL);
                           //Sending the message
    result->len = mavlink_msg_to_send_buffer(result->data, &msg);
}