
#include "Gamepad.h"

Gamepad::Gamepad()
    : deadzone(DEADZONE_DEFAULT)
{
    controller = NULL;
    for (auto& f : axisValues)
        f = 0.0f;

    for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++) {
        buttons[i] = GAMEPAD_BUTTON_NOT_PRESSED;
    }
}

Gamepad::~Gamepad()
{
    close();
}

int Gamepad::getButtonValue(int button)
{
    if (SDL_GameControllerGetButton(controller, (SDL_GameControllerButton)button) == SDL_PRESSED) {
        if (buttons[button] == GAMEPAD_BUTTON_PRESSED) {
            buttons[button] = GAMEPAD_BUTTON_HOLDING;
        } else if (buttons[button] == GAMEPAD_BUTTON_NOT_PRESSED || buttons[button] == GAMEPAD_BUTTON_RELEASED) {
            buttons[button] = GAMEPAD_BUTTON_PRESSED;
        }
    } else if (SDL_GameControllerGetButton(controller, (SDL_GameControllerButton)button) == SDL_RELEASED) {
        if (buttons[button] == GAMEPAD_BUTTON_RELEASED) {
            buttons[button] = GAMEPAD_BUTTON_NOT_PRESSED;
        } else {
            buttons[button] = GAMEPAD_BUTTON_RELEASED;
        }
    } else {
        buttons[button] = GAMEPAD_BUTTON_NOT_PRESSED;
    }

    return buttons[button];
}

float Gamepad::getAxisValue(int i)
{
    return axisValues[i];
}

bool Gamepad::isConnected()
{
    return(controller != NULL);
}

bool Gamepad::close()
{
    if (controller != NULL)
    {
        SDL_GameControllerClose(controller);
        controller = NULL;
        return true;
    }

    return false;
}

float Gamepad::getAxisValue(STICK_AXIS stick)
{
    return axisValues[stick];
}

bool Gamepad::getState()
{
    if (controller != NULL)
    {
        for (int i = 0; i < SDL_CONTROLLER_AXIS_MAX; ++i)
        {
            Sint16 value = SDL_GameControllerGetAxis(controller, (SDL_GameControllerAxis)(i));

            if ((value < -deadzone || value > deadzone))
            {
                value > 0 ? axisValues[i] = value / 32767.0f : axisValues[i] = value / 32768.0f;
            }
            else
            {
                axisValues[i] = 0.0;
            }
        }

        return true;
    }
    else
    {
        return false;
    }
}

void Gamepad::rumble(float strength, int ms)
{
    if (controller != NULL)
    {
        SDL_HapticRumblePlay(haptic, strength, ms);
    }
}

bool Gamepad::open()
{
    //
    controller = SDL_GameControllerOpen(NULL);

    if (controller != 0)
    {
        joystick = SDL_GameControllerGetJoystick(controller);
        haptic = SDL_HapticOpenFromJoystick(joystick);
        SDL_HapticRumbleInit(haptic);
    }

    //
    return(controller != NULL);
}