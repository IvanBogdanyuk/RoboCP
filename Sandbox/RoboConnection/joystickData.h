#pragma once
#include "QtCore\QWaitCondition.h"
#include "QtCore\qthread.h"
#include "QtCore\QMutex"
#include "QTCore\qelapsedtimer.h"
#include "MAVLinkTest\mavlink_types.h"
#include <iostream>

#include <ctime>
typedef unsigned short uint16_t;
const uint16_t RUDDER_DEFAULT = 0, GAS_DEFAULT = 0, PITCH_DEFAULT = 0, ROLL_DEFAULT = 0;

class MavlinkPacket {
public:
	unsigned char* data;
	uint16_t len;

	MavlinkPacket();
	void toString();
};

class MavlinkVisitor {
public:
	virtual void visitHeartBeat(MavlinkPacket* result) = 0;
	virtual void visitRc_Channels_Override(MavlinkPacket* result, unsigned short pitch, unsigned short roll, unsigned short gas, unsigned short rudder) = 0;
};

class MavlinkMessage {
public:
	virtual void toMavlinkPacket(MavlinkPacket* result, MavlinkVisitor* visitor) = 0;
};

class HeartBeat : public MavlinkMessage{
public:
	virtual void toMavlinkPacket(MavlinkPacket* result, MavlinkVisitor* visitor);
};

class JoystickData : public MavlinkMessage{
public:
	uint16_t rudder;
	uint16_t gas;
	uint16_t pitch;
	uint16_t roll;
public:
	JoystickData(uint16_t a, uint16_t b, uint16_t c, uint16_t d) :rudder(a), gas(b), pitch(c), roll(d){};
	JoystickData() :rudder(RUDDER_DEFAULT), gas(GAS_DEFAULT), pitch(PITCH_DEFAULT), roll(ROLL_DEFAULT){};

	virtual void toMavlinkPacket(MavlinkPacket* result, MavlinkVisitor* visitor);

	JoystickData* clone();

	void copy(JoystickData* to);

	void print();
};

class MavlinkBuffer{
public:
	virtual void writeJoystickData(JoystickData* jdata) = 0;
	virtual void read(MavlinkPacket* packet, MavlinkVisitor* visitor) = 0;
};

class SingleJoystickBuffer : public MavlinkBuffer{
	JoystickData* jbuffers;
	bool activeBuffer;

	bool isReading;
	QMutex mutex;
	QWaitCondition readingCondition;
	QElapsedTimer heartBitTimer, joystickTimer;
	HeartBeat* heartbeat;

	long lastHeartbeat;
public:
	SingleJoystickBuffer();

	void switchBuffer();

	void writeJoystickData(JoystickData* jdata);

	void readJoystickData(MavlinkPacket* packet, MavlinkVisitor* visitor);

	void read(MavlinkPacket* packet, MavlinkVisitor* visitor);

	~SingleJoystickBuffer();
};

class CircularJoystickBuffer : public MavlinkBuffer
{
	JoystickData* firstBuffer;	//buffer to send
	JoystickData* secondBuffer;	//queue to send
	bool* firstSent;	//sent messages in f-t buffer
	bool* secondSent;

	JoystickData midJoystickData;	//for computing mean

	int size;
	int i, j;	//place to add in first and second buffs
	bool isReading;
	bool writeToSecondBuffer;
	QMutex firstMutex;
	QMutex secondMutex;
	HeartBeat* heartbeat;

	long lastHeartbeat;
public:
	CircularJoystickBuffer(int array_size);
	void flushToFirstBuffer();

	void writeJoystickData(JoystickData* jData);
	void getMiddleValue(JoystickData* midJoystickData, int i);
	void fill(bool *firstSent);
	void readJoystickData(JoystickData* midJoystickData);
	void readJoystickData(MavlinkPacket* packet, MavlinkVisitor* visitor);
	void read(MavlinkPacket* packet, MavlinkVisitor* visitor);
	~CircularJoystickBuffer();
};

class Joystick {
public:
	virtual void getJoysticState(JoystickData* data) = 0;
};