
#pragma once

#include "InteractionLayer.h"

class OptionsWindowType 
{
private:
    LocalAssetManager * package;
    Button2D windowModes[2];
    Text2D windowMode;
    Window * window;
    SDL_Color colour1;
    SDL_Color colour2;
public:
    OptionsWindowType(Window * window);
    ~OptionsWindowType();

    void onGamepadButton(int key, int state, int a);
    void create(LocalAssetManager *);
    void render(Renderer2D *);
    void event(SDL_Event&);

    void makeActive(unsigned int);
};