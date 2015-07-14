#pragma once
#include "joystickData.h"
#include "robotLink.h"
#include "mavlink_vis.h"

#include <SDL.h>

int sent = 0, read = 0;

//mock implementations of a joysick and com-port connection

class MockJoystick : public Joystick{
public:
	MockJoystick(){
		srand(time(NULL));
	}
	virtual void getJoysticState(JoystickData* data){
		data->rudder = rand();
		data->gas = rand();
		data->pitch = rand();
		data->roll = rand();

		read++;
	}
};

class MockRobotLinker : public RobotLinker{
	long timer;
public:
	MockRobotLinker(){ timer = time(NULL); }
	virtual void sendPacket(MavlinkPacket* packet){
		//packet->toString();
		sent++;
		if (time(0) > timer + 3){
			std::cout << "rate: " << (1.0*read) / sent << "\n";
			timer = time(0);
		}
			
		(QThread::currentThread())->msleep(4);
	}
	virtual void openPort(QString name){

	}
};

class CircularBufferTestSuite{
	CircularJoystickBuffer* testBuffer;
	JoystickData* jData;
	MavlinkPacket* packet;
	MavlinkVisitor* visitor;
public:
	CircularBufferTestSuite()
	{
		testBuffer = new CircularJoystickBuffer(10);
		packet = new MavlinkPacket();
		visitor = new ComMavlinkVisitor();
	}
	bool simpleTest(){
		jData = new JoystickData(1600, 1550, 1570, 1600);
		testBuffer -> writeJoystickData(jData);
		jData->rudder = 1450;
		jData->gas = 1475;
		jData->pitch = 1465;
		jData->roll = 1450;
		testBuffer -> writeJoystickData(jData);
		jData->rudder = 1450;
		jData->gas = 1475;
		jData->pitch = 1465;
		jData->roll = 1450;
		testBuffer -> writeJoystickData(jData);
		testBuffer->readJoystickData(jData);
		std::cout << jData->rudder << ' ' << jData->pitch << ' ' << jData->roll << ' ' << jData->gas << std :: endl;
		
	}
	bool run(){
		return simpleTest();
	}
};