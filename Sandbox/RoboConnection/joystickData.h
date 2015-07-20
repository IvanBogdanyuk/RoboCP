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

	bool isGasZero();
	bool isNonZero();

	JoystickData* clone();
	void copy(JoystickData* to);
	void print();
	void setZero();
};

class ControlBuffer{
public:
	virtual JoystickData* Read() = 0;
	virtual void Next() = 0;
	virtual void Write(JoystickData* data) = 0;
	virtual int Size() = 0;
	virtual int GetMaxSize() = 0;
};

class DataOutputController{
public:
	virtual void Read(MavlinkPacket* packet, MavlinkVisitor* visitor) = 0;
	virtual void SetControlBuffer(ControlBuffer* buffer) = 0;
};

class DataInputController
{
public:
	virtual void WriteJoystickData(JoystickData* jdata) = 0;
	virtual void SetControlBuffer(ControlBuffer* buffer) = 0;
};

/*class SingleJoystickBuffer : public DataInputController, DataOutputController{
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

class CircularJoystickBuffer : public DataInputController, DataOutputController
	//synchronized buffer with circular structure and a queue for writing
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
*/

class JoystickDataPool{
public:
	JoystickDataPool(int size);
	JoystickData* GetJoystickData();
	void FreeJoystickData(JoystickData* data);
	~JoystickDataPool();
private:
	int m_size;
	bool* m_freeData;
	JoystickData* m_data;
	int m_lastHandled;
};

class QueuedControlBuffer : public ControlBuffer{
public:
	QueuedControlBuffer(int max_size);
	JoystickData* Read();
	void Next();
	void Write(JoystickData* data);
	int Size();
	int GetMaxSize();

private:
	TSDataHandler<JoystickData*>* m_joystickDataQueue;
	JoystickDataPool* m_dataPool;

	int m_size;
	JoystickData* m_workData;
};

class JoystickToBufferController : public DataInputController{
public:
	JoystickToBufferController(ControlBuffer* buffer);
	void WriteJoystickData(JoystickData* jdata);
	void SetControlBuffer(ControlBuffer* buffer);

private:
	ControlBuffer* m_buffer;
};

class BufferToLinkerController : public DataOutputController{
public:
	BufferToLinkerController(ControlBuffer* buffer, int rate);
	void Read(MavlinkPacket* packet, MavlinkVisitor* visitor);
	void SetControlBuffer(ControlBuffer* buffer);

private:
	JoystickData* prepareJData();

	ControlBuffer* m_buffer;

	JoystickData* m_meanJoystickData;
	JoystickData* m_workData;
	int m_rate;

	QElapsedTimer m_heartBitTimer;
	HeartBeat* m_heartbeat;
};

class Joystick {
public:
	bool isDanger();
	bool hasBegun();
	virtual void GetJoysticState(JoystickData* data) = 0;

protected:
	void checkData(JoystickData* data);

	int m_began;
	int m_danger;
};

class CrossStabilizer : public Joystick
{
public:
	virtual DataHandler<CrossPoint2D>* GetPointContainer() = 0;
};

class CrossPoint2D{
public:
	CrossPoint2D();
	CrossPoint2D(CrossPoint2D& point);
	CrossPoint2D& operator=(CrossPoint2D &arg);

	double GetX();
	double GetY();
	void SetXY(double x, double y);
private:
	double m_x, m_y;
};

class SimpleProportionalCrossStabilizer : public CrossStabilizer
{
public:
	SimpleProportionalCrossStabilizer(double factor);
	bool isDanger();
	bool hasBegun();
	void GetJoysticState(JoystickData* data);

	DataHandler<CrossPoint2D>* GetPointContainer();

private:
	double m_factor;
	DataHandler<CrossPoint2D>* m_pointContainer;

	CrossPoint2D m_workPoint;
};

class ControlSwitcher : public CrossStabilizer{
public:
	ControlSwitcher(CrossStabilizer* stabilizer, Joystick* externJoystick);
	DataHandler<CrossPoint2D>* GetPointContainer();
	void GetJoysticState(JoystickData* data);

private:
	CrossStabilizer* m_stabilizer;
	Joystick* m_externJoystick;
};