
#pragma once

#include "OptionsGraphics.h"
#include "OptionsVolume.h"
#include "OptionsWindowSize.h"
#include "OptionsWindowType.h"

class OptionsField
{
    OptionsWindowSize optionsWindowSizeActor;
    OptionsWindowType optionsWindowTypeActor;
    OptionsGraphics optionsGraphicsActor;
    OptionsVolume optionsVolumeActor;

    LocalAssetManager * package;
    Window * window;
    int active;
public:
    explicit OptionsField(Window *);
    ~OptionsField();

    void onGamepadButton(int k, int s);
    void onGameEvent(SDL_Event& e);
    void render(Renderer2D *);
    void create(LocalAssetManager*);
    void update();
};