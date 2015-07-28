#pragma once
#include "joystickData.h"
#include "jbuffer_test.h"
#include "JoysticThreads.h"
#include "ComRobotLinker.h"
#include "joystick.h"
#include <QtCore\QCoreApplication>
#include <QtCore\qdebug.h>
#include <QtCore\qobject.h>

#include <QtSerialPort\QSerialPort>
#include <QtSerialPort\QSerialPortInfo>
#include "mavlink_vis.h"

class ArducopterControlSystemImpl : public ArducopterControlSystem
{
	Q_OBJECT
public:
	ArducopterControlSystemImpl(RobotLinker* linker);
	void redButtonPressed(bool pressed);
	void keyPressed(char key);
	void joystickButtonPressed(char button);
	void setLastSendedJoystickData(JoystickData* data);
	void switchControl(int control);
	int arm(bool* ready);
	void reconnect(bool* ready);

	void start();
	DataHandler<CrossPoint2D>* GetPointContainer();

	bool isRedButtonPressed();
	JoystickData* lastSendedJDataPointer();

private:
	bool m_redButtonPressed;
	JoystickData m_jdata;

	Joystick* m_joystick;
	CrossStabilizer* m_stabilizer;
	ControlSwitcher* m_switcher;

	ControlBuffer* m_controlBuffer;
	JoystickToBufferController* m_inputController;
	DataOutputController* m_outputController;

	RobotLinker* m_link;
	MavlinkVisitor* m_mavlinkvisitor;

	JoystickThread* m_jthread;
	RobotLinkThread* m_rthread;

	long m_lastControlSended;
	QElapsedTimer m_timer;
};