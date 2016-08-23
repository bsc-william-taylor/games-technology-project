
#pragma once

enum class SceneID 
{
    Intro, 
    Menu, 
    Indoors, 
    Forest, 
    Gameover, 
    Options
};

#include "InteractionLayer.h"
#include "gameover.h"
#include "options.h"
#include "forest.h"
#include "indoors.h"
#include "intro.h"
#include "menu.h"

#pragma comment(lib, "Silence.Core.lib")
#pragma comment(lib, "Silence.Api.lib")
