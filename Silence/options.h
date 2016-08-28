
#pragma once

#include "System.h"
#include "Actors.h"

class Options : public Scene
{
    LocalAssetManager * package;
    OptionsField * optionInputs;
    SceneManager * scenes;

    Renderer2D renderer2D;
    Texture2D background;
    Button2D backButton;
    Text2D version;
    Text2D header;
public:
    explicit Options(OperatingSystem * os);
    ~Options();

    void onGamepadButton(int k, int s) override;
    void onGameEvent(SDL_Event& e) override;
    void onCreate() override;
    void onUpdate() override;
    void onRender() override;
};
