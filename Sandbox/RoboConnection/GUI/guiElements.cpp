#include "guiElements.h"
#include "moc_guiElements.moc"

#include <QtWidgets\qboxlayout.h>

Progress::Progress(QWidget* pobj) : QWidget(pobj), m_nStep(0)
{
	m_bar = new QProgressBar;
	m_bar->setRange(0, 1);
	m_bar->setMinimumWidth(150);
	m_bar->setAlignment(Qt::AlignCenter);

	m_label = new QLabel("Ready");

	QHBoxLayout* layout = new QHBoxLayout;
	layout->addWidget(m_label);
	layout->addWidget(m_bar);
	setLayout(layout);

	m_isDone = new bool();
	*m_isDone = true;
}

void Progress::slotStep()
{
	m_nStep++;
	m_bar->setValue(m_nStep);
}

void Progress::slotReset(int numOfSteps)
{
	m_nStep = 0;
	m_bar->reset();
	m_bar->setRange(0, numOfSteps);
}

void Progress::setText(QString & text)
{
	m_label->setText(text);
}

bool* Progress::getDonePointer()
{
	return m_isDone;
}

JoystickWidget::JoystickWidget(QWidget* pwgt) : QWidget(pwgt), 
m_blackPen(Qt::black, 0.5, Qt::SolidLine),
m_redBrush(Qt::red, Qt::SolidPattern), m_whiteBrush(Qt::white, Qt::SolidPattern), m_yellowBrush(Qt::yellow, Qt::SolidPattern),
m_greenBrush(Qt::green, Qt::SolidPattern)
{
	m_pitch = m_roll = m_gas = m_rudder = 1500;
}


void JoystickWidget::setState(unsigned short rudder, unsigned short gas, unsigned short pitch, unsigned short roll)
{
	m_pitch = pitch;
	m_roll = roll;
	m_gas = gas;
	m_rudder = rudder;

	repaint();
}

void JoystickWidget::paintEvent(QPaintEvent* event)
{
	m_painter = new QPainter(this);
	m_painter->setViewport(0, 0, width(), height());
	m_painter->setWindow(0, 0, 100, 100);
	m_painter->setRenderHint(QPainter::Antialiasing);

	m_painter->setBrush(m_whiteBrush);
	m_painter->drawRect(0, 0, 100, 100);
	m_painter->setPen(m_blackPen);

	m_painter->setBrush(m_redBrush);
	m_painter->drawPie(5, 20, 60, 60, 90 * 16, -(m_rudder - 1500)*120/500 * 16);

	m_painter->setBrush(m_whiteBrush);
	m_painter->drawEllipse(10, 25, 50, 50);

	m_painter->drawRect((int)(35 - sqrt(2) * 12), (int)(50 - sqrt(2) * 12), (int)(sqrt(2) * 24.5), (int)(sqrt(2) * 24.5));

	m_painter->setBrush(m_yellowBrush);
	int radius = 3;
	m_painter->drawEllipse((int)((m_pitch - 1500)*(sqrt(2) * 12 - radius)/ 500 + 35 - radius), (int)((1500 - m_roll)*(sqrt(2) * 12 - radius) / 500 + 50 - radius),
		2*radius, 2*radius);

	m_painter->setBrush(m_whiteBrush);
	m_painter->drawRect(75, 20, 10, 60);

	m_painter->setBrush(m_greenBrush);
	if(m_gas>1003) m_painter->drawRect(76, 21 + (2000 - m_gas) * 58 / 1000, 8, (m_gas - 1000) * 58 / 1000);
}