#pragma once

#include "joystickData.h"
#include <SDL.h>

class RealJoystick : public Joystick
{
	SDL_Joystick* joy;
	SDL_Event event;
public:
	RealJoystick()
	{
		// ������������� SDL ��� ������������� ��������� 
		SDL_Init(SDL_INIT_JOYSTICK);
		// ��������
		SDL_JoystickEventState(SDL_ENABLE);
		// ��������� ;)
		joy = SDL_JoystickOpen(0);
	}
	virtual void getJoysticState(JoystickData* data)
	{
		SDL_PollEvent(&event);

		data -> pitch = SDL_JoystickGetAxis(joy, 0);
		data -> roll = SDL_JoystickGetAxis(joy, 1);
		data -> rudder = SDL_JoystickGetAxis(joy, 2);
		data -> gas = SDL_JoystickGetAxis(joy, 3);
	}
};