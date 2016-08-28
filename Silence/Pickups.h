
#pragma once

#include "EngineLayer.h"
#include "md2Model.h"
#include "LocalAssetManager.h"
#include "ForwardRenderer.h"

class Player;

class Pickups
{
    Matrices matrices;
    Player * player;
    Model rocks[2];
    Music pickup3;
    
    PointLight pointLights[2];
    vec3 position[2];
    bool draw[2];
public:
    Pickups();
    ~Pickups();

    void getLights(Lights *);
    void create(LocalAssetManager * package, World&, Player *);
    void render(ForwardRenderer& renderer);
    void update(Camera *);
};