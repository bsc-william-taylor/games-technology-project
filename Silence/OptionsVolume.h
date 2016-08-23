
#pragma once

#include "InteractionLayer.h"

class OptionsVolume
{
    LocalAssetManager * package;
    Button2D volumeSizeButtons[2];
    Text2D volumeSizeLabel;
    Text2D volumeSize;

    SDL_Color colour;
    int volume;
public:
    OptionsVolume();
    ~OptionsVolume();

    void onGamepadButton(int key, int state, int a);
    void create(LocalAssetManager *);
    void render(Renderer2D *);
    void event(SDL_Event&);
    void update();
};