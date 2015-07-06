#pragma once
#include "joystickData.h"
#include "robotLink.h"
#include<ctime>
#include <SDL.h>
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
	virtual void getJoysticState(JoystickData* data)
	{
		SDL_PollEvent(&event);
		data -> pitch = 1000 + (((SDL_JoystickGetAxis(joy, 0) + 32768) *1000)/65536);
		data -> roll = 2000 - (((SDL_JoystickGetAxis(joy, 1) + 32768) *1000)/65536);
		data -> gas = 2000 - (((SDL_JoystickGetAxis(joy, 2) + 32768) *1000)/65536);
		data -> rudder = 1000 + (((SDL_JoystickGetAxis(joy, 3) + 32768) *1000)/65536);
	}
};