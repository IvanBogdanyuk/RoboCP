#pragma once
#include <iostream>
#include <QtWidgets\qapplication.h>
#include <QtWidgets\qwidget.h>
#include <QtWidgets\qpushbutton.h>
#include <QtWidgets\qboxlayout.h>
#include <QtWidgets\qplaintextedit.h>

#include "guiElements.h";
#include "../ControlSystem.h"

class ArducopterGUI : public QObject{
	Q_OBJECT
public:
	ArducopterGUI(int& argc, char** argv, ArducopterControlSystem* controlSystem);
	void show();

public slots:
	void connectPressed();
	void armPressed();
	void switchPressed();
	void redButtonPressed();
	void debugAppend(QString &text);
	void debugClear();

	void lastSendedControl(JoystickData* control);

private:
	QPushButton* m_connect;
	QPushButton* m_arm;
	QPushButton* m_controlSwitcher;
	QPushButton* m_redButton;

	JoystickWidget* m_centerElement;
	QPlainTextEdit* m_videoStream;
	QPlainTextEdit* m_logElement;
	Progress* m_bottomElement;

	QApplication* m_app;
	QWidget* m_wgt;

	ArducopterControlSystem* m_controlSystem;

	int m_controlMode;
};