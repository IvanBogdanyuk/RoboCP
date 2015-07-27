#include "arducopterGUI.h"
#include "moc_arducopterGUI.moc"

ArducopterGUI::ArducopterGUI(int& argc, char** argv, ArducopterControlSystem* controlSystem)
{
	m_controlMode = ControlSwitcher::AUTO;
	m_controlSystem = controlSystem;

	m_app = new QApplication(argc, argv);
	m_wgt = new QWidget();

	m_connect = new QPushButton("connect");
	m_arm = new QPushButton("arm");
	m_controlSwitcher = new QPushButton("switch to joystick");
	m_redButton = new QPushButton("red button");
	m_redButton->setFixedSize(75, 75);

	m_centerElement = new JoystickWidget();
	m_centerElement->setState(1000, 1000, 1050, 1300);
	m_centerElement->setFixedSize(300, 300);
	m_videoStream = new QPlainTextEdit();
	m_logElement = new QPlainTextEdit();
	m_bottomElement = new Progress();
	m_bottomElement->slotReset(10);

	QBoxLayout* mainLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	QBoxLayout* buttonsLayout = new QBoxLayout(QBoxLayout::TopToBottom);
	QBoxLayout* notButtonsPartLayout = new QBoxLayout(QBoxLayout::TopToBottom);
	QBoxLayout* bottomPartLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	QBoxLayout* centerPartLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	QBoxLayout* leftPartLayout = new QBoxLayout(QBoxLayout::TopToBottom);

	buttonsLayout->addWidget(m_connect);
	buttonsLayout->addWidget(m_arm);
	buttonsLayout->addWidget(m_controlSwitcher);
	buttonsLayout->addWidget(m_redButton);

	leftPartLayout->addWidget(m_videoStream);
	leftPartLayout->addWidget(m_logElement);

	bottomPartLayout->addWidget(m_bottomElement);

	centerPartLayout->addLayout(leftPartLayout, 1);
	centerPartLayout->addWidget(m_centerElement, 2);
	notButtonsPartLayout->addLayout(centerPartLayout, 1);
	notButtonsPartLayout->addLayout(bottomPartLayout, 1);
	mainLayout->addLayout(notButtonsPartLayout, 3);
	mainLayout->addLayout(buttonsLayout, 1);

	m_wgt->setLayout(mainLayout);

	m_wgt->resize(600, 400);

	void(QPushButton::*button_pressed)(void) = &QPushButton::pressed;
	void(ArducopterGUI::*arm_receiver)(void) = &ArducopterGUI::armPressed;
	QObject::connect(m_arm, button_pressed, this, arm_receiver);

	void(ArducopterGUI::*connect_receiver)(void) = &ArducopterGUI::connectPressed;
	QObject::connect(m_connect, button_pressed, this, connect_receiver);

	void(ArducopterGUI::*switch_receiver)(void) = &ArducopterGUI::switchPressed;
	QObject::connect(m_controlSwitcher, button_pressed, this, switch_receiver);

	void(ArducopterGUI::*red_button_receiver)(void) = &ArducopterGUI::redButtonPressed;
	QObject::connect(m_redButton, button_pressed, this, red_button_receiver);

	void(ArducopterGUI::*control_changed_receiver)(JoystickData*) = &ArducopterGUI::lastSendedControl;
	void(ArducopterControlSystem::*control_changed_sender)(JoystickData*) = &ArducopterControlSystem::controlSended;
	//QObject::connect(m_controlSystem, control_changed_sender, this, control_changed_receiver);
}

void ArducopterGUI::show()
{
	m_wgt->show();
	m_app->exec();
}

void ArducopterGUI::armPressed()
{
	m_bottomElement->setText(QString("arming..."));
	m_bottomElement->slotReset(m_controlSystem->arm(m_bottomElement->getDonePointer())/1000);
}

void ArducopterGUI::connectPressed()
{
	m_bottomElement->setText(QString("reconnecting..."));
	m_bottomElement->slotReset(1);

	m_controlSystem->reconnect(m_bottomElement->getDonePointer());
}

void ArducopterGUI::switchPressed()
{
	if (m_controlMode == ControlSwitcher::AUTO)
	{
		m_controlSwitcher->setText("Joystick");
		m_controlSystem->switchControl(ControlSwitcher::JOYSTICK);
		m_controlMode = ControlSwitcher::JOYSTICK;
	}
	else if (m_controlMode == ControlSwitcher::JOYSTICK)
	{
		m_controlSwitcher->setText("CrossDetection");
		m_controlSystem->switchControl(ControlSwitcher::CROSS);
		m_controlMode = ControlSwitcher::CROSS;
	}
	else if (m_controlMode == ControlSwitcher::CROSS)
	{
		m_controlSwitcher->setText("AUTO");
		m_controlSystem->switchControl(ControlSwitcher::AUTO);
		m_controlMode = ControlSwitcher::AUTO;
	}
}
void ArducopterGUI::redButtonPressed()
{
	m_controlSystem->redButtonPressed(true);
	m_bottomElement->setText(QString("stopped."));
}
void ArducopterGUI::debugAppend(QString &text)
{
	m_logElement->appendPlainText(text);
}
void ArducopterGUI::debugClear()
{
	m_logElement->clear();
}

void ArducopterGUI::lastSendedControl(JoystickData* control)
{
	m_centerElement->setState(control->rudder, control->gas, control->pitch, control->roll);
}