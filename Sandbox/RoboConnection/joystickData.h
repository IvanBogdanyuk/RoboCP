#pragma once
#include "QtCore\QWaitCondition.h"
#include "QtCore\qthread.h"
#include "QtCore\QMutex"
#include "QtCore\QQueue.h"
#include "QTCore\qelapsedtimer.h"
#include "MAVLinkTest\mavlink_types.h"

#include "TSDataHandler.h"
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
	virtual void VisitHeartBeat(MavlinkPacket* result) = 0;
	virtual void VisitRc_Channels_Override(MavlinkPacket* result, unsigned short pitch, unsigned short roll, unsigned short gas, unsigned short rudder) = 0;
	virtual void VisitRc_Channels_Override(MavlinkPacket* result, JoystickData* data) = 0;
};

class MavlinkMessage {
public:
	virtual void ToMavlinkPacket(MavlinkPacket* result, MavlinkVisitor* visitor) = 0;
};

class HeartBeat : public MavlinkMessage{
public:
	virtual void ToMavlinkPacket(MavlinkPacket* result, MavlinkVisitor* visitor);
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

	virtual void ToMavlinkPacket(MavlinkPacket* result, MavlinkVisitor* visitor);

	JoystickData* clone();

	void copy(JoystickData* to);

	void print();

	void setZero();
};

class DataController{
public:
	virtual int writeJoystickData(JoystickData* jdata) = 0;
	virtual void read(MavlinkPacket* packet, MavlinkVisitor* visitor) = 0;
};

class SingleJoystickBuffer : public DataController{
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

	int writeJoystickData(JoystickData* jdata);

	void readJoystickData(MavlinkPacket* packet, MavlinkVisitor* visitor);

	void read(MavlinkPacket* packet, MavlinkVisitor* visitor);

	~SingleJoystickBuffer();
};

class CircularJoystickBuffer : public DataController
	/*synchronized buffer with circular structure and a queue for writing*/
{
	JoystickData* firstBuffer;	//buffer to send
	JoystickData* secondBuffer;	//queue to write
	bool* firstSent;	//sent messages-cells in f-t buffer
	bool* secondSent;	// -//-

	JoystickData midJoystickData;	//for computing mean

	int size;	//length of a buffer
	int i, j;	//place to add in first and second buffs
	bool isReading;	//flag that raised when reading from buffer
	bool writeToSecondBuffer;	//flag that raised when write to the queue
	QMutex firstMutex;	
	QMutex secondMutex;

	HeartBeat* heartbeat;	//message that should be sent at least one time per second

	int secondBuffLoad;	//num of ele
	double factorToWait;

	QElapsedTimer heartBitTimer, joystickTimer;
	long lastHeartbeat;
public:
	CircularJoystickBuffer(int array_size);
	void flushToFirstBuffer();

	int writeJoystickData(JoystickData* jData);
	void getMiddleValue(JoystickData* midJoystickData, int i);
	void fill(bool *firstSent);
	void readJoystickData(JoystickData* midJoystickData);
	void readJoystickData(MavlinkPacket* packet, MavlinkVisitor* visitor);
	void read(MavlinkPacket* packet, MavlinkVisitor* visitor);
	~CircularJoystickBuffer();
};

class JoystickDataPool{
public:
	JoystickDataPool(int size);
	JoystickData* GetJoystickData();
	void FreeJoystickData(JoystickData* data);
	~JoystickDataPool();
private:
	int m_size;
	bool* m_freeData;
	JoystickData** m_data;
	int m_lastHandled;
};

class DataSeparateController : public DataController{
	
public:
	DataSeparateController(int size);
	int WriteJoystickData(JoystickData* jdata);
	void Read(MavlinkPacket* packet, MavlinkVisitor* visitor);
	~DataSeparateController();
private:
	JoystickData* prepareJData();

	TSDataHandler<JoystickData*>* m_joystickDataQueue;

	JoystickDataPool* m_dataPool;
	JoystickData* m_meanJoystickData;
	JoystickData* m_workData;
	int m_size;
};

class Joystick {
public:
	virtual bool isDanger() = 0;
	virtual bool hasBegun() = 0;
	virtual void GetJoysticState(JoystickData* data) = 0;
protected:
	bool Danger;
	bool Began;
};