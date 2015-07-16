#include "robo_mavlink_test\mavlink.h"

#include "joystickData.h"
#pragma once
class ComMavlinkVisitor : public MavlinkVisitor
{
public:
	ComMavlinkVisitor();
	virtual void visitHeartBeat(MavlinkPacket* result);
	virtual void visitRc_Channels_Override(MavlinkPacket* result, unsigned short pitch, unsigned short roll, unsigned short gas, unsigned short rudder);
private:							//MAVLink protocol, message ID #0
	mavlink_system_t mavlink_system;//ID and component
	mavlink_system_t mavlink_target;

	uint8_t system_type;
	uint8_t autopilot_type;
	uint8_t system_mode;
	uint32_t custom_mode;
	uint8_t system_state;
};