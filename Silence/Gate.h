
#pragma once

#include "EngineLayer.h"
#include "ForwardRenderer.h"
#include "Model.h"
#include "LocalAssetManager.h"
#include "SceneManager.h"
#include "Key.h"

class Gate
{
    Matrices matrices;
    Model gate;

    bool exited;
    bool leave;
public:
    Gate();
    ~Gate();

    void create(LocalAssetManager *, World& world, SceneManager * scene);
    void render(ForwardRenderer&);
    void update(Key * key);
    void reset();

    bool hasHit();
};