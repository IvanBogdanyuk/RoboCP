#include "joystick.h"


RealJoystick::RealJoystick()
{
	SDL_Init(SDL_INIT_JOYSTICK);
	SDL_JoystickEventState(SDL_ENABLE);
	joy = SDL_JoystickOpen(0);
}
uint16_t RealJoystick::convert(int JData, bool toinvert)
{
	if (toinvert)
		return (uint16_t)(2000 - (JData + 32767) / 65.536);
	else
		return (uint16_t)(1000 + (JData + 32768) / 65.536);
}
void RealJoystick::getJoysticState(JoystickData* data)
{
	SDL_PollEvent(&event);
	data->pitch = convert(SDL_JoystickGetAxis(joy, 0), false);
	data->roll = convert(SDL_JoystickGetAxis(joy, 1), true);
	data->gas = convert(SDL_JoystickGetAxis(joy, 2), true);
	data->rudder = convert(SDL_JoystickGetAxis(joy, 3), false);
}
