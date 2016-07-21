
/**
*
* Copyright (c) 2014 : William Taylor : wi11berto@yahoo.co.uk
*
* This software is provided 'as-is', without any
* express or implied warranty. In no event will
* the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute
* it freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented;
*    you must not claim that you wrote the original software.
*    If you use this software in a product, an acknowledgment
*    in the product documentation would be appreciated but
*    is not required.
*
* 2. Altered source versions must be plainly marked as such,
*    and must not be misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source distribution.
*/


#ifndef __LVL__H_
#define __LVL__H_

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
private:
    LocalAssetManager * package;
    ForwardRenderer renderer3D;
    Renderer2D renderer2D;
    SceneManager * manager;
    Window * window;
    Player player;
    World world;

    bool exiting;

    Music torchSound;
    Music pickup1;
    Music pickup2;
    Music pickup3;
    Music jumpSound;
    Music doorSound;
    Music indoorMusic;

    Matrices matrices;

    DirectionalLight sun;
    PointLight lights[5];
    SpotLight torch;

    Gamepad * gamepad;
    Hints hints;
public:
    Indoors(OperatingSystem * os);
    ~Indoors();

    Player * getPlayer();
    Model * getKey();

    void onGamepadButton(int key, int st);
    void onGamepadAxis(int, float);
    void onGameEvent(SDL_Event& e);
    void onEnter(int);
    void onExit(int);
    void onCreate();
    void onUpdate();
    void onRender();
private:
    Cube cube;
    
    AnimatedModel * monster;
    AnimatedModel * bank;
    AnimatedModel * bed;

    Model key;
    Model body;
    Model model;
    Model door;
    Model torchLight;
    Model rocks;

    AnimatedModel * wagon;
    AnimatedModel * generator;
    AnimatedModel * switchboard;

    float alpha;
    bool showTorchLight;
    bool showRocks;
    bool show;
private:
    void generateRandomLights();
    void generateWorld();
};

#endif