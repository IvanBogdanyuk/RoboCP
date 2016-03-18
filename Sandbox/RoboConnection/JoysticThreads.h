#ifndef __JOYSTICK_THREADS_H__
#define __JOYSTICK_THREADS_H__


#include "QtCore\qthread.h"
#include "QtCore\qstring.h"
#include "QtCore\qelapsedtimer.h"

#include "com_connection.h"
#include "robotLink.h"
#include "CopterOutput.h"
#include "joystickData.h"
#include "preprocess.h"
#include "WebcamCapture.h"

class JoystickThread : public QThread
{
public:
    JoystickThread(Joystick* joystick, JoystickToBufferController* buffer, ArducopterControlSystem* controlSystem);
    void run();    
private:
    JoystickData data;
    Joystick* joystick;
    JoystickToBufferController* buffer;

	ArducopterControlSystem* m_controlSystem;
};

class RobotLinkThread : public QThread{
public:
	RobotLinkThread(DataOutputController* buffer, RobotLinker* link, MavlinkVisitor* visitor, ArducopterControlSystem* system);
    void run();
    void sleep_m(int millis);
private:
    DataOutputController* m_buffer;
    RobotLinker* m_link;
    MavlinkPacket m_packet;
    MavlinkVisitor* m_visitor;
	ArducopterControlSystem* m_controlSystem;

	int m_count;
	
	void Stop();
};

class RobotLinkerReadingThread : public QThread
{
public:
	RobotLinkerReadingThread(RobotLinker* linker, DataInputController* controller);
	void run();

private:
	RobotLinker* m_robotLinker;
	DataInputController* m_inputController;
	MavlinkPacket* m_packet;
};

class CameraDbgThread : public QThread
{
public:
	CameraDbgThread(DataHandler<CrossPoint2D> *dh_out = NULL, TSDataHandler<Mat> *dbg_outputImage = NULL);
	void run();

private:
	TSDataHandler<Mat> *m_dbg_outputImage;
	DataHandler<CrossPoint2D> *m_DataHandler_out;
	cv::Mat img;
};

#endif