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

	MockCrossDetector* crossDetector = new MockCrossDetector(switcher->GetPointContainer());
    JoystickThread* jthread = new JoystickThread(switcher, inputController);    //thread that reads joystick state and pushes it to the buffer
    RobotLinkThread* rthread = new RobotLinkThread(outputController, link, mavlinkvisitor, switcher);    //thread that reads the latest joystick state to a buffer and sends it via com-port
	crossDetector->start();
	jthread->start();
    rthread->start();
	
	/*
#pragma region CrossDetection
    // инициализация очереди на обработку изображения
    TSDataHandler* cap2proc = new TSDataHandler();
    // инициализация очереди для вывода 
    TSDataHandler* proc2out = new TSDataHandler();
    cv::Mat img;

    // инициализация и старт потоков считывания и обработки данных
    WebcamCapture capThread(cap2proc);
    ProcessingThread procThread(cap2proc, proc2out);
    capThread.start();
    procThread.start();

    // цикл вывода обработанных изображений
    forever
    {
        int key = -1;
        if (proc2out->ReadFrame(img))
        {
            cv::imshow("Output", img);
            key = cv::waitKey(1);
            // если нажат пробел, то break
            if (key == 27)
                break;
        }
    }

    // закрытие потоков
    capThread.exit();
    procThread.exit();
#pragma endregion




    getchar();
#ifdef PROFILING
    std::cout<<"Profiling statistics: \n";
    std::cout<<"joystick reading total time: "<<joystickTime<<"\n";
    std::cout << "mean time: " << joystickTime / joystickTimes<<"\n\n";

    std::cout << "com port writing total time: " << robotLinkTime << "\n";
    std::cout << "mean time: " << robotLinkTime / robotLinkTimes << "\n\n";
#endif

	*/
    getchar();	
    return 0;	
}
