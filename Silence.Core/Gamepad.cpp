
#include "Gamepad.h"

Gamepad::Gamepad() :
    controller(nullptr),
    joystick(nullptr),
    haptic(nullptr),
    deadzone(DEADZONE_DEFAULT)
{
    for (auto& f : axisValues)
    {
        f = 0.0f;
    }

    for (auto i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++) 
    {
        buttons[i] = GamepadButtonNotPressed;
    }
}

Gamepad::~Gamepad()
{
    close();
}

int Gamepad::getButtonValue(int button)
{
    auto controlButton = static_cast<SDL_GameControllerButton>(button);

    if (SDL_GameControllerGetButton(controller, controlButton) == SDL_PRESSED)
    {
        if (buttons[button] == GamepadButtonPressed) 
        {
            buttons[button] = GamepadButtonHolding;
        } 
        else if (buttons[button] == GamepadButtonNotPressed || buttons[button] == GamepadButtonReleased) 
        {
            buttons[button] = GamepadButtonPressed;
        }
    } 
    else if (SDL_GameControllerGetButton(controller, controlButton) == SDL_RELEASED)
    {
        if (buttons[button] == GamepadButtonReleased) 
        {
            buttons[button] = GamepadButtonNotPressed;
        } 
        else 
        {
            buttons[button] = GamepadButtonReleased;
        }
    } 
    else 
    {
        buttons[button] = GamepadButtonNotPressed;
    }

    return buttons[button];
}

float Gamepad::getAxisValue(int i)
{
    return axisValues[i];
}

bool Gamepad::isConnected() const
{
    return controller != nullptr;
}

bool Gamepad::close()
{
    if (controller != nullptr)
    {
        SDL_GameControllerClose(controller);
        controller = nullptr;
        return true;
    }

    return false;
}

float Gamepad::getAxisValue(ControllerStickAxis stick)
{
    return axisValues[stick];
}

bool Gamepad::getState()
{
    if (controller != nullptr)
    {
        for (auto i = 0; i < SDL_CONTROLLER_AXIS_MAX; ++i)
        {
            auto axis = static_cast<SDL_GameControllerAxis>(i);
            auto value = SDL_GameControllerGetAxis(controller, axis);

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
    
    return false;
}

void Gamepad::rumble(float strength, int ms) const
{
    if (controller != nullptr)
    {
        SDL_HapticRumblePlay(haptic, strength, ms);
    }
}

bool Gamepad::open()
{
    controller = SDL_GameControllerOpen(NULL);

    if (controller != nullptr)
    {
        joystick = SDL_GameControllerGetJoystick(controller);
        haptic = SDL_HapticOpenFromJoystick(joystick);

        if(haptic != nullptr)
        {
            SDL_HapticRumbleInit(haptic);
        }
    }

    return controller != nullptr;
}