
#pragma once

#include "InteractionLayer.h"
#include "HLGI.h"
#include "Monster.h"

class Player 
{
private:
    bool pickedUpRocks;
    bool pickedUpTorch;

    HighPrecisionTimer throwTimer;

    Music throwSound;
    Music hitSound;
public:
    Player();
    ~Player();

    void onGamepadButton(FirstPersonCamera * camera, int key, int state);
    void onKeyEvent(FirstPersonCamera * camera, int key, int state);
    void setTorchPickupState(bool);
    void setRockPickupState(bool);
    void create(LocalAssetManager * package);
    void update(FirstPersonCamera * camera, Monster * monster);
    void replenishRocks();
    void reset();

    bool hasTorch();
    bool hasRocks();
private:
    int rocks;
    int timeout;
};