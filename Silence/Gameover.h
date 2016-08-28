
#pragma once

#include "System.h"
#include "HLGI.h"

class Gameover : public Scene
{
    HighPrecisionTimer * survivorTimer;
    LocalAssetManager * package;
    SceneManager * scenes;
    
    Renderer2D renderer2D;
    Texture2D background;
    Texture2D boxart;
    
    Button2D back;
    
    Text2D time;
    Text2D header;
    Text2D message;

    std::string timeSurvived;
public:
    explicit Gameover(OperatingSystem * os);
    ~Gameover();

    void onGamepadButton(int k, int s) override;
    void onGameEvent(SDL_Event& e) override;
    void onCreate() override;
    void onUpdate() override;
    void onRender() override;
    void onEnter(int i) override;
};
