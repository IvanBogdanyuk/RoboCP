#pragma once
#include "joystickData.h"
#include "QtCore\qthread.h"
#include "QtCore\qstring.h"


class MyThread : public QThread
{
public:
	// constructor
	// set name using initializer
	explicit MyThread(QString s);

	// overriding the QThread's run() method
	void run();
private:
	QString name;
};





