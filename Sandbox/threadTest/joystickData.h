#pragma once
#include "QtCore\QWaitCondition.h"
#include "QtCore\qthread.h"
#include "QtCore\QMutex"
#include <iostream>

const int RUDDER_DEFAULT = 0, GAS_DEFAULT = 0, PITCH_DEFAULT = 0, ROLL_DEFAULT = 0;



class JoystickData{
public:
	int rudder;
	int gas;
	int pitch;
	int roll;
public:
	JoystickData(int a, int b, int c, int d) :rudder(a), gas(b), pitch(c), roll(d){};
	JoystickData() :rudder(RUDDER_DEFAULT), gas(GAS_DEFAULT), pitch(PITCH_DEFAULT), roll(ROLL_DEFAULT){};

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

class JoystickBuffer{
	JoystickData* jbuffers;
	bool activeBuffer;

	bool isReading;
	QMutex mutex;
	QWaitCondition readingCondition;

public:
	JoystickBuffer() {
		jbuffers = new JoystickData[2];
		activeBuffer = 0;

		isReading = false;
	}

	void switchBuffer(){
		if (activeBuffer) activeBuffer = 0;
		else activeBuffer = 1;
	}

	void write(JoystickData* jdata){
		jdata->copy(&jbuffers[activeBuffer]);

		mutex.lock();

		while(isReading) readingCondition.wait(&mutex);
		switchBuffer();
		
		mutex.unlock();
	}

	void read(JoystickData* jdata){
		
		mutex.lock();
		isReading++;

		jbuffers[activeBuffer].copy(jdata);

		isReading -= 1;
		readingCondition.wakeAll();
		mutex.unlock();

		
	}

	~JoystickBuffer(){
		delete[] jbuffers;
	}
};

class Joystick {
public:
	virtual void getJoysticState(JoystickData* data) = 0;
};