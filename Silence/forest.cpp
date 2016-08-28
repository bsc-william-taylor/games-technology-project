
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

#include "Forest.h"
#include "Silence.h"

// Constructor & Deconstructor
Forest::Forest(OperatingSystem * engine)
{
    manager = engine->acquireSceneManager();
    gamepad = engine->acquireGamepad();
    
    package = engine->acquireAssetManager()->grabLocalManager();
    package->grab({ "data/fonts/Calibri.ttf",
        "data/models/house/br_house1.md3",
        "data/textures/grass.png",
        "data/textures/dirt.png",
        "data/textures/pathways.png",
        "data/models/wall/wall.obj",
        "data/models/tree/tree1a_lod0.obj",
        "data/textures/ai_path2.png",
        "data/models/gate/SecurityGate.obj",
        "data/models/rocks/obj.obj"
    });

    backgroundMusic.open(package->newAudio("data/media/main.wav", StreamLoop));
    torchSound.open(package->newAudio("data/media/torch.mp3", Load));
}

Forest::~Forest()
{
    // Package needs to be deleted by the scene itself
    SAFE_RELEASE(package);
}

void Forest::onGamepadButton(int key, int state)
{
    if (key == SDL_CONTROLLER_BUTTON_BACK && state == GamepadButtonPressed)
    {
        manager->exit();
    }

    if (key == SDL_CONTROLLER_BUTTON_X && state == GamepadButtonPressed) {
        if (player->hasTorch()) {
            torchSound.reset();
            torchSound.play();
            torch.toggle();
        }
    }

    player->onGamepadButton((FirstPersonCamera *)renderer3D.getCamera(), key, state);
}

void Forest::onGamepadAxis(int i, float x)
{
    renderer3D.onGamepadAxis(i, x);
}

// Handles when the scene is created
void Forest::onCreate()
{
    matrices.makeProjection3D(60.0f, glm::vec2(0.1f, 350.0f));
    terrain.create(package, world);
    monster.create(package);
    house.create(package);
    gate.create(package, world, manager);
    
    // then set the initial camera for the scene and its position & the direction light
    renderer3D.createRenderer();
    renderer3D.changeCamera(CameraType::FirstPerson);
    renderer3D.setCameraDirection(180, 0);
    renderer3D.setCameraArea(glm::vec4(-7500.0f, -2500.0f, 7500.0, 2500.0f));
    renderer3D.setCameraPosition(glm::vec3(0.0, 8.0, -35.0));

    Lights * lights = renderer3D.getLights();

    nightLight.setDirection(glm::vec3(0.0, -1.0, -0.3));
    nightLight.setColour(glm::vec3(1.4, 0.7, 0.7));
    nightLight.turnOn();

    torch.setDirection(vec3(0.0, 0.0, 1.0));
    torch.setPosition(vec3(0.0, 0.0, 0.0));
    torch.setColour(vec3(0.75, 0.75, 0.75));
    torch.setAttribuation(0.02);
    torch.setConeAngle(25.0);

    player = ((Indoors *)manager->getScene((int)SceneID::Indoors))->getPlayer();

    if (player->hasTorch()) {
        torch.turnOn();
    } else {
        torch.turnOff();
    }

    point.setPosition(glm::vec3(key.getPosition()));
    point.setColour(glm::vec3(0.35, 0.2, 0.0));
    point.setAttribuation(glm::vec4(0.1f, 0.3f, 0.007f, 0.0016f));
    point.turnOn();

    auto func = [&](Camera * c, SolidBox *) {
        c->cancelMovement();
    };

    world.onHit(new SolidBox(glm::vec3(-78.0f, -1.0, -28.0f), glm::vec3(78.0f, 1.0, 28.0f)), func);
    world.onHit(new SolidBox(glm::vec3(-75.0f, -1.0, 48.0f), glm::vec3(-15.0f, 1.0, 53.0f)), func);
    world.onHit(new SolidBox(glm::vec3(12, -1.0, 48.0f), glm::vec3(75, 1.0, 53.0f)), func);

    world.setPlayerCamera(renderer3D.getCamera());
    
    key.create(((Indoors *)manager->getScene((int)SceneID::Indoors))->getKey(), package);
    key.spawn(world, &point);

    pickups.create(package, world, player);
    pickups.getLights(lights);

    lights->pushDirectionalLight(&nightLight);
    lights->pushPointLight(&point);
    lights->pushSpotLight(&torch);

    footSteps.open(package->newAudio("data/media/footsteps.mp3", LoadLoop));
    sign.open(package->newAudio("data/media/sign.mp3", Load));
    roar.open(package->newAudio("data/media/roar.mp3", Load));
}

// handle SDL events
void Forest::onGameEvent(SDL_Event& e)
{
    // pass each SDL event to the renderer which will update the camera
    renderer3D.updateCamera(e);
    terrain.event(e);
    
    if (e.key.keysym.sym == SDLK_f && e.type == SDL_KEYUP) {
        if (player->hasTorch()) {
            torchSound.reset();
            torchSound.play();
            torch.toggle();
        }
    }

    player->onKeyEvent((FirstPersonCamera *)renderer3D.getCamera(), e.key.keysym.sym, e.type);
}

// Handle os updates
void Forest::onUpdate()
{	
    auto camera = (FirstPersonCamera *)renderer3D.getCamera();

    camera->prepareCamera();

    terrain.update(renderer3D);
    gate.update(&key);
    house.update();

    player->update(camera, &monster);

    if (alpha < 1.0f) {
        alpha += 0.005f;
    }

    monster.update(renderer3D.getCamera());
    pickups.update(camera);

    if (monster.isCloseToPlayer(renderer3D.getCamera()->getPosition())) {
        sign.play();
    }
    else {
        sign.reset();
    }

    if (monster.getTravel() >= 1.0) {
        roar.play();
    }

    if (monster.isActive()) {
        float length = glm::length(glm::distance(monster.getPosition(), renderer3D.getCamera()->getPosition()));

        if (gamepad->isConnected() && timer.elapsed(TimeType::Milliseconds) >= length) {
            gamepad->rumble(0.75, 100);
            timer.clear();
            timer.start();
        }
    }
    else {
        if (gamepad->isConnected() && timer.elapsed(TimeType::Milliseconds) >= 1000) {
            gamepad->rumble(0.75, 100);
            timer.clear();
            timer.start();
        }
    }

    if (monster.hasKilledPlayer(renderer3D.getCamera()->getPosition())) {
        manager->switchScene((int)SceneID::Gameover);
    }
    else {
        if (camera->getSpeed() < 0.01) {
            footSteps.pause();
        } else {
            footSteps.play();
        }
    }

    // then re position the camera
    renderer3D.repositionCamera();
    world.updateWorld();
}

void Forest::onEnter(int i)
{
    renderer3D.setCameraPosition(glm::vec3(0.0, 8.0, -35.0));
    renderer3D.setCameraDirection(180, 0);

    backgroundMusic.play();
    renderer3D.enableFog();
    roar.reset();
    monster.reset();
    survivalTimer.start();
    footSteps.reset();
    alpha = 0.0f;

    timer.start();
}

// Render the scene for the user
void Forest::onRender()
{ 
    renderer3D.prepare();
    renderer3D.setProjectionMatrix(matrices);
    renderer3D.setModelMatrix(matrices);
    renderer3D.setAlpha(alpha);

    terrain.render(renderer3D);
    monster.render(renderer3D);
    house.render(renderer3D);
    key.render(renderer3D);
    gate.render(renderer3D);
    pickups.render(renderer3D);

    renderer3D.present();
}

void Forest::onExit(int i)
{
    SDL_SetRelativeMouseMode(SDL_FALSE);

    backgroundMusic.stop();
    footSteps.pause();
    roar.reset();
    roar.play();

    player->reset();
    gate.reset();
    key.reset();
}