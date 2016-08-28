
#pragma once

#include "EngineLayer.h"
#include "AnimatedModel.h"
#include "LocalAssetManager.h"
#include "ForwardRenderer.h"
#include "Ai.h"

class Intelligence 
{
    double direction;
    double travel;
    PathMap * map;
    vec3 tempDest;
public:
    Intelligence();
    ~Intelligence();

    double getTravel();
    double getDirection();

    void setTemporaryDestination(vec3, vec3);
    bool applyUpdate(vec3& position, FirstPersonCamera * camera);
    void setup(LocalAssetManager * manager);
    void reset();
    void end();

    vec3 toSceneSpace(vec3);
};