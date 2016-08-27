
#pragma once

#include "EngineLayer.h"

#define DEADZONE_DEFAULT 6000

enum ControllerStickAxis
{
    LeftX, 
    LeftY, 
    RightX, 
    RightY,
};

enum ControllerButtonState 
{
    GamepadButtonPressed,
    GamepadButtonHolding,
    GamepadButtonReleased,
    GamepadButtonNotPressed
};

class SILENCE_EXPORT Gamepad 
{
    SDL_GameController * controller;
    SDL_Joystick * joystick;
    SDL_Haptic * haptic;

    ControllerButtonState buttons[SDL_CONTROLLER_BUTTON_MAX];
    float axisValues[SDL_CONTROLLER_AXIS_MAX];
    int deadzone;
public:
    Gamepad();
    virtual ~Gamepad();

    int getButtonValue(int axisNumber);

    float getAxisValue(ControllerStickAxis axis);
    float getAxisValue(int axis);

    void rumble(float strength, int ms) const;

    bool isConnected() const;
    bool getState();
    bool close();
    bool open();
};

