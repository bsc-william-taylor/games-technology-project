
#pragma once

#include "InteractionLayer.h"
#include "Actors.h"
#include "Border.h"
#include "ModelSet.h"
#include "Trees.h"

class Terrain
{
private:
    Heightmap terrain;
    Matrices matrices;
    Border border;
    Trees trees;
public:
    Terrain();
    ~Terrain();

    void create(LocalAssetManager *, World&);
    void render(ForwardRenderer&);
    void update(ForwardRenderer&);
    void event(SDL_Event& e);
}; 