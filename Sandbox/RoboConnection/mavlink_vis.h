#include "robo_mavlink_test\mavlink.h"

#include "joystickData.h"
#pragma once
class ComMavlinkVisitor : public MavlinkVisitor
{
public:
    ComMavlinkVisitor();
    virtual void VisitHeartBeat(MavlinkPacket* result);
    virtual void VisitRc_Channels_Override(MavlinkPacket* result, unsigned short pitch, unsigned short roll, unsigned short gas, unsigned short rudder);
    virtual void VisitRc_Channels_Override(MavlinkPacket* result, JoystickData* data);
private:                              //MAVLink protocol, message ID #0
    mavlink_system_t m_mavlink_system;//ID and component
    mavlink_system_t m_mavlink_target;

    uint8_t m_system_type;
    uint8_t m_autopilot_type;
    uint8_t m_system_mode;
    uint32_t m_custom_mode;
    uint8_t m_system_state;
};