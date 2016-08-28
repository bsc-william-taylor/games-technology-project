
#pragma once

#include "EngineLayer.h"
#include "md2Model.h"
#include "LocalAssetManager.h"
#include "ForwardRenderer.h"
#include "Intelligence.h"

class Monster
{
    Intelligence intelligence;
    AnimatedModel * enemy;
    Matrices matrices;
    bool updated;
    float alpha;
public:
    Monster();
    ~Monster();

    bool hasKilledPlayer(vec3);
    bool isCloseToPlayer(vec3);
    bool isActive();

    void create(LocalAssetManager * package);
    void render(ForwardRenderer& renderer);
    void update(Camera *);
    void distract(vec3);
    void reset();

    float getTravel();

    vec3 to_scene_space(vec3);
    vec3 getPosition();
private:
    vec3 cameraPosition;
    vec3 position;
    float enemyDirection;
    int spawnID;
};