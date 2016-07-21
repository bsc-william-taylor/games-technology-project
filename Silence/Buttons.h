
#pragma once

#include "InteractionLayer.h"

class Buttons
{
private:
    LocalAssetManager * package;
    unsigned int active;
    Button2D buttons[3];
    Music roarSound;

public:
    Buttons();
    ~Buttons();

    void onGamepadButton(int button, int state);
    void create(LocalAssetManager *);
    void render(Renderer2D *);
    void event(SDL_Event&);
    void update();
    void enter();

    bool isPressed(int buttonNumber, int i, int s);
    bool isPressed(int i, SDL_Event& e);
private:
    void load(int active);
};