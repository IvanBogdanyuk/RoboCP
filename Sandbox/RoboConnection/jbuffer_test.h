#pragma once
#include "joystickData.h"
#include "robotLink.h"
#include "mavlink_vis.h"
#include<ctime>
#include <SDL.h>

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
	}
};

class MockRobotLinker : public RobotLinker{
public:
	MockRobotLinker(){}
	virtual void sendPacket(MavlinkPacket* packet){
		packet->toString();

	};
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