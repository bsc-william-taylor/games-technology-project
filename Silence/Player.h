
#pragma once

#include "InteractionLayer.h"
#include "HLGI.h"
#include "Monster.h"

class Player 
{
    HighPrecisionTimer throwTimer;
    Music throwSound;
    Music hitSound;

    bool pickedUpRocks;
    bool pickedUpTorch;

    int timeout;
    int rocks;
public:
    Player();
    ~Player();

    void onGamepadButton(FirstPersonCamera * camera, int key, int state);
    void onKeyEvent(FirstPersonCamera * camera, int key, int state);

    void update(FirstPersonCamera * camera, Monster * monster);
    void create(LocalAssetManager * package);
    void replenishRocks();
    void reset();

    void setTorchPickupState(bool);
    void setRockPickupState(bool);

    bool hasTorch();
    bool hasRocks();
};