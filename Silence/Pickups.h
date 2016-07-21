


#pragma once

#include "EngineLayer.h"
#include "md2Model.h"
#include "LocalAssetManager.h"
#include "ForwardRenderer.h"
#include "Ai.h"
#include "Intelligence.h"

class Player;

class Pickups
{
private:
    Matrices matrices;
    Music pickup3;
    Model rocks[2];
    Player * player;
    PointLight pointLights[2];
public:
    Pickups();
    ~Pickups();

    void getLights(Lights *);

    void create(LocalAssetManager * package, World&, Player *);
    void render(ForwardRenderer& renderer);
    void update(Camera *);
private:
    bool draw[2];
    vec3 position[2];
};