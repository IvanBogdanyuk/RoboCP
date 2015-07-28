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
#include "ControlSystem.h"
#include "GUI\arducopterGUI.h"

int main(int argc, char *argv[])
{
	
	RobotLinker* linker = new ComRobotLinker();	//connection with robot

	ArducopterControlSystemImpl* controlSystem = new ArducopterControlSystemImpl(linker);	//control producing system

	CopterOutputBuffer* copterOutputBuffer = new OneElementCopterOutputBuffer();	//responce reading system
	CopterOutputController* copterOutputController = new CopterOutputControllerImpl(copterOutputBuffer);
	DataInputController* linkerToBufferController = new LinkerToBufferController(copterOutputBuffer);
	RobotLinkerReadingThread* linkReadingThread = new RobotLinkerReadingThread(linker, linkerToBufferController);

	ArducopterGUI* gui = new ArducopterGUI(argc, argv, controlSystem);
	
	TSDataHandler<Mat>* dbg_outputImage = new TSDataHandler<Mat>();
    TSDataHandler<Mat>* cap2proc = new TSDataHandler<Mat>();
    DataHandler<CrossPoint2D>* proc2out = controlSystem->GetPointContainer();
    WebcamCapture capThread(cap2proc, 0);
	ProcessingThread procThread(cap2proc, proc2out, dbg_outputImage);
    capThread.start();
    procThread.start();

	CameraDbgThread* camera_dbg = new CameraDbgThread(proc2out, dbg_outputImage);

	
	controlSystem->start();
	//linkReadingThread->start();
	camera_dbg->start();
	gui->show();

	//debug cycle
	/*while (true)
	{
		CrossPoint2D out;
		if (proc2out->Peek(out) && dbg_outputImage->Read(img))
		{
			//cout << "[" << out.GetX() << ';' << out.GetY() << "]" <<  endl;
			imshow("OUTPUT", img);
			waitKey(1);
		}
	}	*/

    getchar();	
    return 0;	
}
