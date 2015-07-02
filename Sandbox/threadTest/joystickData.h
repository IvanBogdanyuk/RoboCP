#pragma once

class joystickData{
	joystickData(int a, int b, int c, int d) :rudder(a), gas(b), pitch(c), roll(d){};
	int rudder;
	int gas;
	int pitch;
	int roll;
};