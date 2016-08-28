
#pragma once

#include "EngineLayer.h"
#include "HighPrecisionTimer.h"
#include "LocalAssetManager.h"
#include "Renderer2D.h"
#include "Gamepad.h"

enum HintTypes { KeyHint, RockHint, TorchHint, ExitHint };

class Hints 
{
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
    void throwHint(HintTypes hint, Gamepad *);
    void update();
};