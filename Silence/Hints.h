
#pragma once

#include "EngineLayer.h"
#include "LocalAssetManager.h"
#include "ForwardRenderer.h"
#include "Renderer2D.h"
#include "HighPrecisionTimer.h"
#include "Gamepad.h"

enum HINT_TYPES {
    KEY_HINT,
    ROCK_HINT,
    TORCH_HINT,
    EXIT_HINT,
};

class Hints 
{
private:
    LocalAssetManager * package;
    HighPrecisionTimer fadeoutTimer;
    Text2D hintRenderable;
    std::string hintText;

    float alpha;
public:
    Hints();
    ~Hints();

    void create(LocalAssetManager * package);
    void render(Renderer2D * renderer);
    void throwHint(HINT_TYPES hint, Gamepad *);
    void update();
};