#include <iostream>
#include <vector>

#include "jbuffer_test.h"
#include "JoysticThreads.h"
#include "ComRobotLinker.h"
#include "joystick.h"
#include <QtCore\QCoreApplication>
#include <QtCore\qdebug.h>
 
#include <QtSerialPort\QSerialPort>
#include <QtSerialPort\QSerialPortInfo>
#include "mavlink_vis.h"


#include <opencv2\highgui\highgui.hpp>
#include "WebcamCapture.h"
#include "ProcessingThread.h"
#include "TSDataHandler.h"

int main(int argc, char *argv[])
{
	

    Joystick* joystick = new RealJoystick();    //initializing a joystick
    CrossStabilizer* stabilizer = new SimpleProportionalCrossStabilizer(1.0);
    ControlSwitcher* switcher = new ControlSwitcher(stabilizer, joystick);

    ControlBuffer* controlBuffer = new QueuedControlBuffer(1000);
    DataInputController* inputController = new JoystickToBufferController(controlBuffer);
    DataOutputController* outputController = new BufferToLinkerController(controlBuffer, 10);

    RobotLinker* link = new ComRobotLinker();    //initializing a com-port connection
    MavlinkVisitor* mavlinkvisitor = new ComMavlinkVisitor();    //helps to convert different objects to mavlink packet

    //MockCrossDetector* crossDetector = new MockCrossDetector(switcher->GetPointContainer());
    TSDataHandler<Mat>* cap2proc = new TSDataHandler<Mat>();
    TSDataHandler<CrossPoint2D>* proc2out = switcher->GetPointContainer();
    WebcamCapture capThread(cap2proc, 0);
    ProcessingThread procThread(cap2proc, proc2out);
    capThread.start();
    procThread.start();
    
    JoystickThread* jthread = new JoystickThread(switcher, inputController);    //thread that reads joystick state and pushes it to the buffer
    RobotLinkThread* rthread = new RobotLinkThread(outputController, link, mavlinkvisitor, switcher);    //thread that reads the latest joystick state to a buffer and sends it via com-port
    crossDetector->start();
    jthread->start();
    rthread->start();
    
    getchar();	
    return 0;	
}
