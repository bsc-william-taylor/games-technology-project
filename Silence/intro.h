
#pragma once

#include "HighPrecisionTimer.h"
#include "System.h"

class Intro : public Scene
{
    HighPrecisionTimer introTimer;
    OperatingSystem * system;
    Video introVideo;
public:
    explicit Intro(OperatingSystem * os);
    ~Intro();

    void onGamepadButton(int k, int s) override;
    void onGameEvent(SDL_Event& e) override;
    void onEnter(int) override;
    void onCreate() override;
    void onUpdate() override;
    void onRender() override;
};