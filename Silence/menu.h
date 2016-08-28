
#pragma once

#include "System.h"
#include "HLGI.h"
#include "Media.h"

class Menu : public Scene 
{
    LocalAssetManager * package;
    SceneManager * scenes;
    Renderer2D renderer2D;
    Background background;
    Headers headers;
    Buttons buttons;
    Music menuMusic;
    Music roar;

    bool entered;
    bool exited;

    float alpha;
public:
    explicit Menu(OperatingSystem * os);
    ~Menu();

    void onGamepadButton(int k, int s) override;
    void onGameEvent(SDL_Event& e) override;
    void onCreate() override;
    void onUpdate() override;
    void onRender() override;
    void onEnter(int) override;
};