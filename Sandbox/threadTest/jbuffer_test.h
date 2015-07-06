#pragma once
#include "joystickData.h"
#include "robotLink.h"
#include<ctime>
#include <SDL.h>
typedef  unsigned short uint16_t;
class MockJoystick : public Joystick{
public:
	MockJoystick(){
		srand(time(NULL));
	}
	virtual void getJoysticState(JoystickData* data){
		data->rudder = rand();
		data->gas = rand();
		data->pitch = rand();
		data->roll = rand();
	}
};

class MockRobotLinker : public RobotLinker{
public:
	MockRobotLinker(){}
	void sendJoystickData(JoystickData* data){
		data->print();
		_sleep(1000);
	}
};
class RealJoystick : public Joystick
{
	SDL_Joystick* joy;
	SDL_Event event;
public:
	RealJoystick()
	{
		// Инициализация SDL для использования джойстика 
		SDL_Init(SDL_INIT_JOYSTICK);
		// Включаем
		SDL_JoystickEventState(SDL_ENABLE);
		// Открываем ;)
		joy = SDL_JoystickOpen(0);
	}
	uint16_t convert(int JData, bool toinvert)
	{
		if (toinvert)
			return (uint16_t) (2000 - (JData + 32767)/65.536);
		else
			return (uint16_t) (1000 + (JData + 32768)/65.536);
	}
	virtual void getJoysticState(JoystickData* data)
	{
		SDL_PollEvent(&event);
		data -> pitch = convert(SDL_JoystickGetAxis(joy, 0), false);
		data -> roll = convert(SDL_JoystickGetAxis(joy, 1), true);
		data -> gas = convert(SDL_JoystickGetAxis(joy, 2), true);
		data -> rudder = convert(SDL_JoystickGetAxis(joy, 3), false);
	}
};

