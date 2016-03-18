#pragma once

#include "joystickData.h"
#include <SDL.h>

class RealJoystick : public Joystick
{
	SDL_Joystick* joy;
	SDL_Event event;
public:
	RealJoystick();
	uint16_t convert(int JData, bool toinvert);
	virtual void getJoysticState(JoystickData* data);
};