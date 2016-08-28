
#pragma once

#include "ForwardRenderer.h"
#include "System.h"
#include "World.h"
#include "Media.h"
#include "Actors.h"
#include "Pickups.h"

class Forest : public Scene
{
    HighPrecisionTimer survivalTimer;
    LocalAssetManager * package;
    ForwardRenderer renderer3D;
    SceneManager * manager;
    Gamepad * gamepad;
    Player * player;
    Pickups pickups;
    World world;

    DirectionalLight nightLight;
    PointLight point;
    SpotLight torch;
    
    Music backgroundMusic;
    Music torchSound;
    Music footSteps;
    Music roar;
    Music sign;

    HighPrecisionTimer timer;
    Matrices matrices;
    Terrain terrain;
    Monster monster;
    House house;
    Gate gate;
    Key key;

    float alpha;
public:
    explicit Forest(OperatingSystem * os);
    ~Forest();

    void onEnter(int i) override;
    void onExit(int i) override;
    void onGamepadButton(int key, int st) override;
    void onGamepadAxis(int, float) override;
    void onGameEvent(SDL_Event& e) override;
    void onCreate() override;
    void onUpdate() override;
    void onRender() override;

    HighPrecisionTimer * getForestTime();
};