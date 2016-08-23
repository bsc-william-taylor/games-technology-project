
#pragma once

#include "InteractionLayer.h"

enum GraphicSettings 
{
    HIGH, MEDIUM, LOW
};

class OptionsGraphics
{
    LocalAssetManager * package;
    Window * window;

    GraphicSettings currentSettings;
    Button2D graphicsSizeButtons[2];
    Text2D graphicsizeLabel;
    Text2D graphicsSize;

    SDL_Color colour;
public:
    OptionsGraphics();
    ~OptionsGraphics();

    void onGamepadButton(int key, int state, int a);
    void create(LocalAssetManager *, Window *);
    void render(Renderer2D *);
    void event(SDL_Event&);
    void update();
};