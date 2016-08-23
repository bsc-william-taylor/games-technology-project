

#pragma once

#include "InteractionLayer.h"

class Background 
{
    LocalAssetManager * package;
    HighPrecisionTimer timer;
    Texture2D background;
    float alpha;
    int index;
public:
    Background();
    ~Background();

    void create(LocalAssetManager *);
    void render(Renderer2D *);
    void event(SDL_Event&);
    void update();
    void enter();

    float getAlpha();
};