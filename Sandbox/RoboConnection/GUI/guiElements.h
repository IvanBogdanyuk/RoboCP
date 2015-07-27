#pragma once

#include <QtWidgets\qwidget.h>
#include <QtWidgets\qprogressbar.h>
#include <QtWidgets\qlabel.h>
#include <QtGui\qpainter.h>

class Progress : public QWidget{
	Q_OBJECT
public:
	Progress(QWidget* pobj = 0);
public slots:
	void slotStep();
	void slotReset(int numOfSteps);
	void setText(QString & text);

	bool* getDonePointer();
private:
	QProgressBar* m_bar;
	QLabel* m_label;
	int m_nStep;

	bool* m_isDone;
};

class JoystickWidget : public QWidget{
	Q_OBJECT
public:
	JoystickWidget(QWidget* pwgt = 0);

public slots:
	void setState(unsigned short rudder, unsigned short gas, unsigned short pitch, unsigned short roll);

protected:
	void paintEvent(QPaintEvent* event);
private:

	QPainter* m_painter;
	QPen m_blackPen;

	QBrush m_redBrush;
	QBrush m_whiteBrush;
	QBrush m_yellowBrush;
	QBrush m_greenBrush;

	unsigned short m_pitch, m_roll, m_gas, m_rudder;
};