#pragma once
#include "QtCore\QWaitCondition.h"
#include "QtCore\qthread.h"
#include "QtCore\QMutex"
#include "QTCore\qelapsedtimer.h"
#include "MAVLinkTest\mavlink_types.h"
#include <iostream>

#include <ctime>

const int RUDDER_DEFAULT = 0, GAS_DEFAULT = 0, PITCH_DEFAULT = 0, ROLL_DEFAULT = 0;
typedef unsigned short uint16_t;
class MavlinkPacket {
public:
	unsigned char* data;
	uint16_t len;
	MavlinkPacket(){
		data = new unsigned char[MAVLINK_MAX_PACKET_LEN];
	}
	void toString()
	{
		int length = ((unsigned short)data[1]) + 7;

		for (int i = 0; i < length; i++){
			std::cout << (unsigned short)data[i] << "\t";
		}

		std::cout << "\n";
	}
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
	virtual void toMavlinkPacket(MavlinkPacket* result, MavlinkVisitor* visitor){
		visitor->visitHeartBeat(result);
	}
};

class JoystickData : public MavlinkMessage{
public:
	uint16_t rudder;
	uint16_t gas;
	uint16_t pitch;
	uint16_t roll;
public:
	JoystickData(int a, int b, int c, int d) :rudder(a), gas(b), pitch(c), roll(d){};
	JoystickData() :rudder(RUDDER_DEFAULT), gas(GAS_DEFAULT), pitch(PITCH_DEFAULT), roll(ROLL_DEFAULT){};

	virtual void toMavlinkPacket(MavlinkPacket* result, MavlinkVisitor* visitor){
		visitor->visitRc_Channels_Override(result, pitch, roll, gas, rudder);
	}

	JoystickData* clone(){
		return new JoystickData(rudder, gas, pitch, roll);
	}

	void copy(JoystickData* to){

		to->rudder = rudder;
		to->gas = gas;
		to->pitch = pitch;
		to->roll = roll;
	}

	void print(){
		std::cout << "JoystickData: \nrudder:" << rudder << "\ngas: " << gas << "\npitch: " << pitch << "\nroll: " << roll << "\n\n";
	}
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
	SingleJoystickBuffer() {
		jbuffers = new JoystickData[2];

		activeBuffer = 0;

		isReading = false;

		heartbeat = new HeartBeat();
		lastHeartbeat = -1;
		heartBitTimer.start();
		joystickTimer.start();
	}

	void switchBuffer(){
		if (activeBuffer) activeBuffer = 0;
		else activeBuffer = 1;
	}

	void writeJoystickData(JoystickData* jdata){
		jdata->copy(&jbuffers[activeBuffer]);

		mutex.lock();

		while (isReading) readingCondition.wait(&mutex);
		switchBuffer();

		mutex.unlock();
	}

	void readJoystickData(MavlinkPacket* packet, MavlinkVisitor* visitor){

		mutex.lock();
		isReading++;

		jbuffers[!activeBuffer].toMavlinkPacket(packet, visitor);	//bug

		isReading -= 1;
		readingCondition.wakeAll();
		mutex.unlock();
	}

	void read(MavlinkPacket* packet, MavlinkVisitor* visitor){
		if (heartBitTimer.elapsed() > 1000)
		{
			heartbeat->toMavlinkPacket(packet, visitor);
			heartBitTimer.restart();
		}
		else
			if (joystickTimer.elapsed() > 59)
			{
				readJoystickData(packet, visitor);
				joystickTimer.restart();
			}
	}

	~SingleJoystickBuffer(){
		delete[] jbuffers;
	}
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
	CircularJoystickBuffer(int array_size)
	{
		size = array_size;
		i = 0;
		j = 0;

		firstBuffer = new JoystickData[size];
		secondBuffer = new JoystickData[size];
		firstSent = new bool[size];
		secondSent = new bool[size];
		for(int k = 0; k < size; k++){
			firstSent[k] = 1;
			secondSent[k] = 1;
		}

		isReading = false;
		writeToSecondBuffer = false;
		heartbeat = new HeartBeat();
		lastHeartbeat = -1;
	}
	void flushToFirstBuffer(){
		for (int k = j; k < (j + size); ++k)
		{
			if (!secondSent[k % size])
			{
				secondBuffer[k % size].copy(&firstBuffer[i]);
				firstSent[i] = 0;
				++i;
				i %= size;
				secondSent[k % size] = 1;
				
			}
		}
	}

	void writeJoystickData(JoystickData* jData)
	{
		
		if (isReading)
		{
			secondMutex.lock();
			writeToSecondBuffer = true;
			if (secondSent[j])  //second buffer is not full
			{
				jData -> copy(&secondBuffer[j]);
				secondSent[j] = 0;
				++j;
				j %= size;
				
			}
			else  //second bufer is full
				std::cout << "Second buffer is full!" << std::endl;
			secondMutex.unlock();
		}
		else 
		{
			firstMutex.lock();
			writeToSecondBuffer = true;
			if (firstSent[i]) //firstbuffer is not full
			{
				jData -> copy(&firstBuffer[i]);
				firstSent[i] = 0;
				++i;
				i %= size;
			}
			else
			{
				if (secondSent[j])
				{
					jData -> copy(&secondBuffer[j]);
					secondSent[j] = 0;
					++j;
					j %= size;
				}
				else
					std :: cout << "Second buffer is full!" << std :: endl;
			}
			firstMutex.unlock();
		}
		
	}
	void getMiddleValue(JoystickData* midJoystickData, int i)
	{
		double rudder = 0, pitch = 0, roll = 0, gas = 0;
		int sum = 0;
		i += size;
		for (int k = 0; k < size; ++k)
		{
			if (!firstSent[k])
			{
				rudder += firstBuffer[(i - k - 1) % size].rudder;
				pitch += firstBuffer[(i - k - 1) % size].pitch;
				roll += firstBuffer[(i - k - 1) % size].roll;
				gas += firstBuffer[(i - k - 1) % size].gas;
				++sum;
			}
		}
		if (sum != 0)
		{

			rudder /= sum;
			pitch /= sum;
			roll /= sum;
			gas /= sum;
		}
		else
		{
			rudder = firstBuffer[(i - 1) % size].rudder;
			pitch = firstBuffer[(i - 1) % size].pitch;
			roll = firstBuffer[(i - 1) % size].roll;
			gas = firstBuffer[(i - 1) % size].gas;
		}
		midJoystickData -> rudder = (uint16_t) abs(rudder);
		midJoystickData -> pitch = (uint16_t) abs(pitch);
		midJoystickData -> roll = (uint16_t) abs(roll);
		midJoystickData -> gas = (uint16_t) abs(gas);
	}
	void fill(bool *firstSent)
	{
		int k = size + i - 1;
		while (!firstSent[k % size])
		{
			firstSent[k % size] = 1;
			--k;
		}

	}
	void readJoystickData(JoystickData* midJoystickData)
	{
		firstMutex.lock();
		++isReading;

		
		getMiddleValue(midJoystickData, i);
		
		fill(firstSent);
		
		if (secondSent[(j + size - 1) % size])
		{
			secondMutex.lock();
			flushToFirstBuffer();
			secondMutex.unlock();
		}
			
		else
			firstSent[(i + size - 1) % size] = 0;
		isReading -= 1;

		
		firstMutex.unlock();		

	}
	void readJoystickData(MavlinkPacket* packet, MavlinkVisitor* visitor)
	{
		readJoystickData(&midJoystickData);
		midJoystickData.toMavlinkPacket(packet, visitor);
	}
	void read(MavlinkPacket* packet, MavlinkVisitor* visitor){
		if(lastHeartbeat < time(NULL) - 900){
			heartbeat->toMavlinkPacket(packet, visitor);
		}
		else readJoystickData(packet, visitor);
	}
	~CircularJoystickBuffer()
	{
		delete [] firstBuffer;
		delete [] secondBuffer;		
		delete [] firstSent;
		delete [] secondSent;
	}
};

class Joystick {
public:
	virtual void getJoysticState(JoystickData* data) = 0;
};