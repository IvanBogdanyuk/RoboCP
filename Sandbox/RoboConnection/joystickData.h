#pragma once
#include "QtCore\QWaitCondition.h"
#include "QtCore\qthread.h"
#include "QtCore\QMutex"
#include <iostream>

#include <ctime>

const int RUDDER_DEFAULT = 0, GAS_DEFAULT = 0, PITCH_DEFAULT = 0, ROLL_DEFAULT = 0;

class MavlinkPacket {
public:
	unsigned char* data;
	MavlinkPacket(){
		data = new unsigned char[32];
	}
	void toString()
	{
		int length = ((unsigned short) data[1]) + 7;

		for(int i = 0; i<length; i++){
			std::cout<<(unsigned short) data[i]<<"\t";
		}

		std::cout<<"\n";
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
	int rudder;
	int gas;
	int pitch;
	int roll;
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
		std::cout<<"JoystickData: \nrudder:"<<rudder<<"\ngas: "<<gas<<"\npitch: "<<pitch<<"\nroll: "<<roll<<"\n\n";
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

	HeartBeat* heartbeat;

	long lastHeartbeat;
public:
	SingleJoystickBuffer() {
		jbuffers = new JoystickData[2];

		activeBuffer = 0;

		isReading = false;

		heartbeat = new HeartBeat();
		lastHeartbeat = -1;
	}

	void switchBuffer(){
		if (activeBuffer) activeBuffer = 0;
		else activeBuffer = 1;
	}

	void writeJoystickData(JoystickData* jdata){
		jdata->copy(&jbuffers[activeBuffer]);

		mutex.lock();

		while(isReading) readingCondition.wait(&mutex);
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
		if(lastHeartbeat < time(NULL) - 900){
			heartbeat->toMavlinkPacket(packet, visitor);
		}
		else readJoystickData(packet, visitor);
	}

	~SingleJoystickBuffer(){
		delete[] jbuffers;
	}
};

class CircularJoystickBuffer : public MavlinkBuffer
{
	JoystickData** firstBuffer;
	JoystickData** secondBuffer;
	int size;
	int i, j;
	
	bool isReading;
	QMutex mutex;

	HeartBeat* heartbeat;

	long lastHeartbeat;
public:
	CircularJoystickBuffer(int array_size)
	{
		size = array_size;
		i = 0;
		j = 0;
		firstBuffer = new JoystickData*[size];
		secondBuffer = new JoystickData*[size];

		for(int k = 0; k<size; k++){
			firstBuffer[k] = new JoystickData();
			secondBuffer[k] = new JoystickData();
		}

		isReading = false;
		heartbeat = new HeartBeat();
		lastHeartbeat = -1;
	}
	void flushToFirstBuffer(){
		for (int k = j; k < (j + size); ++k)
		{
			if (secondBuffer[k % size] != NULL)
			{
				secondBuffer[k % size] -> copy(firstBuffer[i]);
				++i;
				i %= size;
				secondBuffer[k % size] = NULL;
			}
		}
	}

	void writeJoystickData(JoystickData* jData)
	{
		
		if (isReading)
		{
			jData -> copy(secondBuffer[j]);
			++j;
			j %= size;
		}
		else
		{
			mutex.lock();
			jData -> copy(firstBuffer[i]);
			++i;
			i %= size;
			mutex.unlock();
		}
		
	}
	void readJoystickData(MavlinkPacket* packet, MavlinkVisitor* visitor)
	{
		mutex.lock();

		++isReading;
		firstBuffer[(i - 1 + size)%size] -> toMavlinkPacket(packet, visitor);

		flushToFirstBuffer();
		isReading -= 1;
		
		mutex.unlock();
	}
	void read(MavlinkPacket* packet, MavlinkVisitor* visitor){
		if(lastHeartbeat < time(NULL) - 900){
			heartbeat->toMavlinkPacket(packet, visitor);
		}
		else readJoystickData(packet, visitor);
	}
	~CircularJoystickBuffer()
	{

		for(int k = 0; k<size; k++){
			delete firstBuffer[k];
			delete secondBuffer[k];
		}

		delete [] firstBuffer;
		delete [] secondBuffer;
	}
};
class Joystick {
public:
	virtual void getJoysticState(JoystickData* data) = 0;
};