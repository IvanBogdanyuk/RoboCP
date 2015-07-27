#include "ControlSystem.h"
#include "moc_ControlSystem.moc"

ArducopterControlSystemImpl::ArducopterControlSystemImpl(RobotLinker* linker)
{
	m_joystick = new RealJoystick(this);    //initializing a joystick
	m_stabilizer = new SimpleProportionalCrossStabilizer(1.0);	//software control producer
	m_switcher = new ControlSwitcher(m_stabilizer, m_joystick);	//switcher between joystick and software control

	m_controlBuffer = new QueuedControlBuffer(10);	//creating the buffer
	m_inputController = new JoystickToBufferControllerImpl(m_controlBuffer);	//controllers
	m_outputController = new BufferToLinkerController(m_controlBuffer, 10, this);

	m_link = linker;    //the com-port connection
	linker->SetControlSystem(this);

	m_mavlinkvisitor = new ComMavlinkVisitor();    //helps to convert different objects to mavlink packet

	m_jthread = new JoystickThread(m_joystick, m_inputController, this);    //thread that reads joystick state and pushes it to the buffer
	m_rthread = new RobotLinkThread(m_outputController, m_link, m_mavlinkvisitor, this);    //thread that reads the latest joystick state to a buffer and sends it via com-port

	m_redButtonPressed = false;
}

void ArducopterControlSystemImpl::redButtonPressed(bool pressed)
{
	m_redButtonPressed = pressed;
}

void ArducopterControlSystemImpl::keyPressed(char key)
{
	m_redButtonPressed = true;
}
void ArducopterControlSystemImpl::joystickButtonPressed(char button)
{
	m_redButtonPressed = true;
}

void ArducopterControlSystemImpl::setLastSendedJoystickData(JoystickData* data)
{
	data->copy(&m_jdata);
	emit controlSended(&m_jdata);
}

void ArducopterControlSystemImpl::switchControl(int control)
{
	m_switcher->setControl(control);
}
int ArducopterControlSystemImpl::arm(bool* ready)
{
	return m_outputController->sendArmingMessages(ready);
}

void ArducopterControlSystemImpl::start()
{
	m_jthread->start();
	m_rthread->start();
}

void ArducopterControlSystemImpl::reconnect(bool* ready)
{
	m_link->GetParamList();
	*ready = true;

	m_redButtonPressed = false;
}

DataHandler<CrossPoint2D>* ArducopterControlSystemImpl::GetPointContainer()
{
	return m_switcher->GetPointContainer();
}

bool ArducopterControlSystemImpl::isRedButtonPressed()
{
	return m_redButtonPressed;
}

JoystickData* ArducopterControlSystemImpl::lastSendedJDataPointer()
{
	return &m_jdata;
}