
#pragma once

#include "InteractionLayer.h"

class Headers
{
private:
    LocalAssetManager * package;
    // The header for the scene
    Text2D header;
    // The header for the scene
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