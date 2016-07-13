
#ifndef _GAMEPAD_H_
#define _GAMEPAD_H_

#include "SDL2\SDL.h"

#define DEADZONE_DEFAULT 6000

enum STICK_AXIS {
	LEFT_X, LEFT_Y, RIGHT_X, RIGHT_Y,
};

enum BUTTON_STATE {
	GAMEPAD_BUTTON_PRESSED,
	GAMEPAD_BUTTON_HOLDING,
	GAMEPAD_BUTTON_RELEASED,
	GAMEPAD_BUTTON_NOT_PRESSED
};

class Gamepad 
{
private:
	SDL_GameController * controller;
	SDL_Joystick * joystick;
	SDL_Haptic * haptic;

	float axisValues[SDL_CONTROLLER_AXIS_MAX];
	
	BUTTON_STATE buttons[SDL_CONTROLLER_BUTTON_MAX];

	int deadzone;
public:
	Gamepad();
	~Gamepad();

	void rumble(float, int);

	int getButtonValue(int);

	float getAxisValue(STICK_AXIS);
	float getAxisValue(int);

	bool isConnected();
	bool getState();
	bool close();
	bool open();
};

#endif
