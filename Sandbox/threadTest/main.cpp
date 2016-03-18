#include <iostream>
#include <vector>

#include "jbuffer_test.h"
#include "JoysticThreads.h"



#include <QtCore\qthread.h>
#include <SDL.h>

#include <QtCore/QDebug>
#undef main
using namespace std;
int main()
{
	RealJoystick* joystick = new RealJoystick();
	JoystickBuffer* buffer = new JoystickBuffer();
	MockRobotLinker* link = new MockRobotLinker();

	JoystickThread* jthread = new JoystickThread(joystick, buffer);
	RobotLinkThread* rthread = new RobotLinkThread(buffer, link);

	jthread->start();
	rthread->start();

	int i;
	std::cin >> i;
	return 0;



	
	 
}
