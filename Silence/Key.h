
#pragma once

#include "EngineLayer.h"
#include "HLGI.h"
#include "ForwardRenderer.h"
#include "LocalAssetManager.h"
#include "Music.h"

class Key
{
    Matrices matrices;
    PointLight * point;
    Model * keyModel;
    Music pickup;
    int spawnID;
    bool taken;
public:
    Key();
    ~Key();

    vec3 getPosition();

    void create(Model * key, LocalAssetManager * manager);
    void spawn(World& world, PointLight * p);
    void render(ForwardRenderer& renderer);
    void update();
    void reset();
    
    bool hasBeenPickedUp();
};