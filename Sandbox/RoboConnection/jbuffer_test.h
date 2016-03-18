#pragma once
#include "joystickData.h"
#include "robotLink.h"
#include "mavlink_vis.h"
#include "TSDataHandler.h"

#include <SDL.h>

//mock implementations of a joysick and com-port connection

class MockJoystick : public Joystick{
public:
    MockJoystick(){
        srand(time(NULL));
    }
    virtual void GetJoysticState(JoystickData* data){
        data->rudder = rand() % 1000 + 1000;
		data->gas = rand() % 1000 + 1100;
		data->pitch = rand() % 1000 + 1000;
		data->roll = rand() % 1000 + 1000;

    }
};

class MockRobotLinker : public RobotLinker{
	long timer;
public:

	MockRobotLinker(){ timer = time(NULL); }
	virtual void SendPacket(MavlinkPacket* packet){
		(QThread::currentThread())->msleep(4);
	}
	virtual void GetParamList()
	{

	}
	virtual bool ReadPacket(MavlinkPacket* packet)
	{
		(QThread::currentThread())->msleep(4);
		return true;
	}

	virtual void OpenPort(QString name)
	{

	}

	virtual void SetControlSystem(ArducopterControlSystem* system)
	{

	}
};

class MockCrossDetector : public QThread
{
public:
	MockCrossDetector(DataHandler<CrossPoint2D>* container) : m_crossPoint(), m_timer()
	{
		m_pointContainer = container;
	}
	
	void run()
	{
		m_timer.start();
		while (true)
		{
			m_crossPoint.SetXY(10 * sin(m_timer.elapsed() / 1000), 10 * cos(m_timer.elapsed() / 1000));
			m_pointContainer->Write(m_crossPoint);
		}
	}

private:
	DataHandler<CrossPoint2D>* m_pointContainer;
	CrossPoint2D m_crossPoint;
	QElapsedTimer m_timer;
};