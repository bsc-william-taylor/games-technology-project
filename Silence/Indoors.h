
#pragma once

#include "HighPrecisionTimer.h"
#include "ForwardRenderer.h"
#include "System.h"
#include "World.h"
#include "Media.h"
#include "HLGI.h"
#include "Actors.h"
#include "Hints.h"

class Indoors : public Scene
{
    LocalAssetManager * package;
    ForwardRenderer renderer3D;
    Renderer2D renderer2D;
    SceneManager * manager;
    Window * window;
    Player player;
    World world;

    bool showTorchLight;
    bool showRocks;
    bool show;
    bool exiting;

    Music torchSound;
    Music pickup1;
    Music pickup2;
    Music pickup3;
    Music jumpSound;
    Music doorSound;
    Music indoorMusic;

    DirectionalLight sun;
    PointLight lights[5];
    Matrices matrices;
    SpotLight torch;
    Gamepad * gamepad;
    Hints hints;
    Cube cube;

    AnimatedModel * switchboard;
    AnimatedModel * generator;
    AnimatedModel * monster;
    AnimatedModel * wagon;
    AnimatedModel * bank;
    AnimatedModel * bed;

    Model torchLight;
    Model rocks;
    Model model;
    Model body;
    Model door;
    Model key;

    float alpha;
public:
    explicit Indoors(OperatingSystem * os);
    ~Indoors();

    Player * getPlayer();
    Model * getKey();

    void onGamepadButton(int key, int st) override;
    void onGamepadAxis(int, float) override;
    void onGameEvent(SDL_Event& e) override;
    void onEnter(int) override;
    void onExit(int) override;
    void onCreate() override;
    void onUpdate() override;
    void onRender() override;
private:
    void generateRandomLights();
    void generateWorld();
};
