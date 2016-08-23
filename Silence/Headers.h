
#pragma once

#include "InteractionLayer.h"

class Headers
{
    LocalAssetManager * package;
    Text2D header;
    Text2D footer;
public:
    Headers();
    ~Headers();

    void create(LocalAssetManager *);
    void render(Renderer2D *);
    void event(SDL_Event&);
    void update();
    void enter();
};