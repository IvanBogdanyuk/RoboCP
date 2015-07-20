#pragma once
#include "joystickData.h"
#include "robotLink.h"
#include "mavlink_vis.h"
#include "TSDataHandler.h"

#include <SDL.h>

int sent = 0, read = 0;

//mock implementations of a joysick and com-port connection

class MockJoystick : public Joystick{
public:
    MockJoystick(){
        srand(time(NULL));
		m_danger = false;
		m_began = false;
    }
    bool isDanger()
    {
        return m_danger;
    }
    bool hasBegun()
    {
        return m_began;
    }
    virtual void GetJoysticState(JoystickData* data){
        data->rudder = rand() % 1000 + 1000;
		data->gas = rand() % 1000 + 1100;
		data->pitch = rand() % 1000 + 1000;
		data->roll = rand() % 1000 + 1000;

        read++;
		checkData(data);
    }
};

class MockRobotLinker : public RobotLinker{
    long timer;
public:
    MockRobotLinker(){ timer = time(NULL); }
    virtual void SendPacket(MavlinkPacket* packet){
        sent++;
        if (time(0) > timer + 3){
            std::cout << "rate: " << (1.0*read) / sent << "\n";
            timer = time(0);
        }
            
        (QThread::currentThread())->msleep(4);
    }
    virtual void OpenPort(QString name)
	{

    }
	virtual void GetParamList()
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