
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

#include "Indoors.h"
#include "Silence.h"

// Constructor & Deconstructor
Indoors::Indoors(OperatingSystem * engine)
{
    // Aquire a pointer to the os's window
    manager = engine->acquireSceneManager();
    window = &engine->acquireWindow();
    gamepad = engine->acquireGamepad();

    // aquire a new local asset manager
    package = engine->acquireAssetManager()->grabLocalManager();

    // and load the following assets
    package->grab({ "data/textures/brick.png",
        "data/models/hang/hang2.md2",
        "data/fonts/Calibri.ttf",
        "data/models/house/br_house1.md3",
        "data/models/torchlight/Linterna.obj",
        "data/models/door/door.obj",
        "data/models/key/key.3ds",
        "data/models/rocks/obj.obj"
    });

    indoorMusic.open(package->newAudio("data/media/demo.wav", Stream));
    jumpSound.open(package->newAudio("data/media/jump.wav", Load));
    doorSound.open(package->newAudio("data/media/door.wav", Load));
    torchSound.open(package->newAudio("data/media/torch.mp3", Load));
    
    pickup1.open(package->newAudio("data/media/pickup1.mp3", Load));
    pickup2.open(package->newAudio("data/media/pickup2.mp3", Load));
    pickup3.open(package->newAudio("data/media/pickup3.mp3", Load));
    
    showTorchLight = false;
    showRocks = false;
    exiting = false;
    show = false;
}

Indoors::~Indoors()
{
    // Package needs to be deleted by the scene itself
    SAFE_RELEASE(package);
}

void Indoors::onGamepadButton(int key, int state)
{
    if (key == SDL_CONTROLLER_BUTTON_BACK && state == GamepadButtonPressed)
    {
        manager->exit();
    }

    if (key == SDL_CONTROLLER_BUTTON_X && state == GamepadButtonPressed)
    {
        if (showTorchLight) {
            torchSound.reset();
            torchSound.play();
            torch.toggle();
        }
    }
}

Player * Indoors::getPlayer()
{
    return &player;
}

void Indoors::onGamepadAxis(int axis, float x)
{
    renderer3D.onGamepadAxis(axis, x);
}

// Handles when the scene is created
void Indoors::onCreate()
{
    player.create(package);
    
    // load the terrain from the height map texture then set its X and Y scales
    cube.setArea(glm::vec3(-100, 0.0, -50), glm::vec3(200.0, 0.25, 100.0), 20);
    cube.setTexture(package->newTexture("data/textures/brick"));
    model.setModel(package->newModel("data/models/house/br_house1"));
    body.setModel(package->newModel("data/models/hang/hang2"));
    door.setModel(package->newModel("data/models/door/door"));
    key.setModel(package->newModel("data/models/key/key"));
    rocks.setModel(package->newModel("data/models/rocks/obj"));
    torchLight.setModel(package->newModel("data/models/torchlight/Linterna"));
    matrices.makeProjection3D(60.0f, glm::vec2(0.1f, 1000.0f));

    // then create our 3D renderer
    renderer3D.createRenderer();
    hints.create(package);

    // then set the initial camera for the scene and its position & the direction light
    renderer3D.changeCamera(CameraType::FirstPerson);
    renderer3D.setCameraArea(glm::vec4(-75.0f, -25.0f, 75.0, 25.0f));
    renderer3D.setCameraPosition(glm::vec3(0.0, 13.0, -25.0));

    // then generate some random lights for the scene
    switchboard = new AnimatedModel("data/models/switchboard/switchboard.md2", "data/models/switchboard/switchboard.png");
    generator = new AnimatedModel("data/models/room/generator.md2", "data/models/room/generator.png");
    wagon = new AnimatedModel("data/models/wagon/wagon.md2", "data/models/wagon/wagon.png");
    bank = new AnimatedModel("data/models/indoors/bank.md2", "data/models/indoors/bank.png");
    bed = new AnimatedModel("data/models/indoors/bed.md2", "data/models/indoors/bed.png");

    generateRandomLights();
    generateWorld();
    alpha = 1.0f;

    renderer2D.createRenderer();
}

// handle SDL events
void Indoors::onGameEvent(SDL_Event& e)
{
    // pass each SDL event to the renderer which will update the camera
    renderer3D.updateCamera(e);

    if (e.key.keysym.sym == SDLK_f && e.type == SDL_KEYUP)
    {
        if (showTorchLight) {
            torchSound.reset();
            torchSound.play();
            torch.toggle();
        }
    }
}

// Handle os updates
void Indoors::onUpdate()
{
    auto camera = (FirstPersonCamera *)renderer3D.getCamera();
    player.update(camera, NULL);

    if (exiting) {
        alpha -= 0.004f;
        if (alpha <= -0.1f) {
            manager->switchScene((int)SceneID::Forest);
        }
    } else {
        alpha = 1.0f;
    }

    camera->prepareCamera();

    world.updateWorld();
    hints.update();

    renderer3D.repositionCamera();
}

void Indoors::onEnter(int)
{
    SDL_SetRelativeMouseMode(SDL_TRUE);
    ((FirstPersonCamera *)renderer3D.getCamera())->enable();

    indoorMusic.play();
    jumpSound.reset();
    jumpSound.play();
    show = false;
}

// Render the scene for the user
void Indoors::onRender()
{
    renderer3D.prepare();
    renderer3D.setProjectionMatrix(matrices);

    matrices.push();
    matrices.scale(glm::vec3(2.5, 2.0, 2.0));
    matrices.translate(glm::vec3(0.0, -1.0, 0.0));
    matrices.rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    renderer3D.setAlpha(alpha);
    renderer3D.setModelMatrix(matrices);
    renderer3D.renderModel(&model);

    matrices.pop();

    float angles[] = { 45.0f, 90.0f, 155.0f };
    float xs[] = { -10.0f, 10.0f, -12.0f };
    float zs[] = { 30.0f, -10.0f, -50.0f };

    for (int i = 0; i < 3; i++)
    {
        matrices.push();
        matrices.scale(glm::vec3(0.3, 0.3, 0.3));
        matrices.rotate(angles[i], glm::vec3(0.0, 1.0, 0.0));
        matrices.translate(glm::vec3(xs[i], 60, zs[i]));

        renderer3D.setModelMatrix(matrices);
        renderer3D.renderModel(&body);

        matrices.pop();
    }

    if (!show)
    {
        matrices.push();
        matrices.translate(glm::vec3(30.0f, 0.4f, -9.0));
        matrices.rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        matrices.scale(glm::vec3(0.5f, 0.5, 0.5f));

        
        renderer3D.setModelMatrix(matrices);
        renderer3D.renderModel(&key);

        matrices.pop();
    }

    matrices.push();

    renderer3D.setModelMatrix(matrices);
    renderer3D.renderCube(&cube);

    matrices.pop();

    matrices.push();
    matrices.translate(glm::vec3(-70.0f, 0.0f, 0.0f));
    matrices.scale(glm::vec3(0.1, 0.1, 0.1));
    matrices.rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    renderer3D.setModelMatrix(matrices);
    renderer3D.renderAnimatedModel(generator);

    matrices.pop();

    matrices.push();
    matrices.scale(glm::vec3(0.6, 0.6, 0.6));
    matrices.translate(glm::vec3(120.0f, 0.0f, 0.0f));
    matrices.rotate(180.0f, glm::vec3(0.0f, -1.0f, 0.0f));
    matrices.rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    renderer3D.setModelMatrix(matrices);
    renderer3D.renderAnimatedModel(switchboard);

    matrices.pop();

    matrices.push();
    matrices.scale(glm::vec3(0.75, 0.75, 0.75));
    matrices.translate(glm::vec3(20.0f, 0.0f, -10.0f));
    matrices.scale(glm::vec3(0.15, 0.15, 0.15));
    matrices.rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    renderer3D.setModelMatrix(matrices);
    renderer3D.renderAnimatedModel(wagon);

    matrices.pop();
    
    if (show)
    {
        matrices.push();
        matrices.scale(glm::vec3(5.0, 3.5, 4.5));
        matrices.translate(glm::vec3(0.0, 1.0, 0.0));
        matrices.translate(glm::vec3(0.0, 0.0, 5.0));
        matrices.rotate(90.0f, glm::vec3(0.0f, -1.0f, 0.0f));
        
        renderer3D.setModelMatrix(matrices);
        renderer3D.renderModel(&door);

        matrices.pop();
    }

    matrices.push();
    matrices.scale(glm::vec3(0.2, 0.2, 0.2));
    matrices.rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    matrices.rotate(90.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    matrices.translate(glm::vec3(100.0, 150.0, 0.0));

    renderer3D.setModelMatrix(matrices);
    renderer3D.renderAnimatedModel(bank);
    
    matrices.pop();

    matrices.push();
    matrices.scale(glm::vec3(0.5, 0.6, 0.5));
    matrices.rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    matrices.rotate(90.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    matrices.translate(glm::vec3(-40, 75.0, 0.0));

    renderer3D.setModelMatrix(matrices);
    renderer3D.renderAnimatedModel(bed);

    matrices.pop();

    if (!showTorchLight) {
        matrices.push();
        matrices.translate(glm::vec3(-25.0, 8.25, -20.0));
        matrices.scale(glm::vec3(0.5, 0.5, 0.5));

        renderer3D.setModelMatrix(matrices);
        renderer3D.renderModel(&torchLight);

        matrices.pop();
    }

    if (!showRocks) {
        matrices.push();
        matrices.translate(glm::vec3(-40.0, 8.0, 20.0));
        matrices.scale(glm::vec3(0.05, 0.05, 0.05));

        renderer3D.setModelMatrix(matrices);
        renderer3D.renderModel(&rocks);

        matrices.pop();

        matrices.push();
        matrices.translate(glm::vec3(-38.0, 8.0, 19.5));
        matrices.scale(glm::vec3(0.05, 0.05, 0.05));

        renderer3D.setModelMatrix(matrices);
        renderer3D.renderModel(&rocks);

        matrices.pop();

        matrices.push();
        matrices.translate(glm::vec3(-42.0, 8.0, 19.0));
        matrices.scale(glm::vec3(0.05, 0.05, 0.05));

        renderer3D.setModelMatrix(matrices);
        renderer3D.renderModel(&rocks);

        matrices.pop();
    }

    renderer3D.present();

    
    glm::mat4 projectionMatrix = glm::mat4();
    glm::mat4 modelMatrix = glm::mat4();

    projectionMatrix = glm::ortho(0.0, 1920.0, 0.0, 1080.0, -1.0, 1.0);

    renderer2D.prepare();
    renderer2D.setMatrixForObject("projection", projectionMatrix);
    renderer2D.setMatrixForObject("model", modelMatrix);

    hints.render(&renderer2D);

    renderer2D.present();
}

void Indoors::generateRandomLights()
{
    int x_pos[] = { -50, 50, 30, -30, -40 };
    int z_pos[] = { 0, 0, -8, -16, 20 };

    float power[] = { 0.0008f, 0.0008f, 0.0016f, 0.0016f, 0.0016f };

    glm::vec3 colours[] = {
        glm::vec3(1.0, 0.0, 0.0),
        glm::vec3(1.0, 0.0, 0.0),
        glm::vec3(0.3, 0.25, 0),
        glm::vec3(0.3, 0.25, 0),
        glm::vec3(0.3, 0.25, 0)
    };

    Lights * sceneLights = renderer3D.getLights();

    //nightLight.setDirection(glm::vec3(0.0, -1.0, 0.3));
    sun.setDirection(glm::vec3(0.0, -1.0, 0.3));
    sun.setColour(glm::vec3(0.5, 0.25, 0.25));
    sun.turnOn();

    for (int i = 0; i < 5; i++)
    {
        float y = 0.0;
        if (i >= 3) {
            y += 14.0f;
        }

        lights[i].setAttribuation(glm::vec4(0.1f, 0.3f, 0.007f, power[i]));
        lights[i].setPosition(glm::vec3(x_pos[i], y, z_pos[i]));
        lights[i].setColour(colours[i]);
        lights[i].turnOn();

        sceneLights->pushPointLight(&lights[i]);
    }

    sceneLights->pushDirectionalLight(&sun);
}

Model * Indoors::getKey()
{
    return &key;
}

void Indoors::generateWorld()
{
    glm::mat4 modelMatrix = glm::mat4();
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-70.0f, 0.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, 0.1, 0.1));
    modelMatrix = glm::rotate(modelMatrix, -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));


    world.setPlayerCamera(renderer3D.getCamera());
    world.onHit(generator->createbox(modelMatrix), [&](Camera * c, SolidBox * b) {
        c->cancelMovement();
    });


    modelMatrix = glm::mat4();
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2, 0.2, 0.2));
    modelMatrix = glm::rotate(modelMatrix, -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, 90.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(100.0, 150.0, 0.0));

    world.onHit(bank->createbox(modelMatrix), [&](Camera * c, SolidBox * b) {
        if (!showTorchLight) {
            hints.throwHint(TORCH_HINT, gamepad);

            player.setTorchPickupState(true);

            torch.setDirection(vec3(0.0, 0.0, 1.0));
            torch.setPosition(vec3(0.0, 0.0, 0.0));
            torch.setColour(vec3(0.75, 0.75, 0.75));
            torch.setAttribuation(0.02);
            torch.setConeAngle(25.0);
            torch.turnOn();

            renderer3D.getLights()->pushSpotLight(&torch);

            lights[3].turnOff();
            showTorchLight = true;
            pickup1.reset();
            pickup1.play();
        }
    });

    modelMatrix = glm::mat4();
    modelMatrix = glm::translate(modelMatrix, glm::vec3(30.0f, 0.4f, -9.0));
    modelMatrix = glm::rotate(modelMatrix, -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
    
    world.onHit(key.createbox(modelMatrix), [&](Camera * c, SolidBox * b) {
        if (!show) {
            hints.throwHint(KEY_HINT, gamepad);
            lights[2].turnOff();
            pickup3.reset();
            pickup3.play();
            show = true;
        }
    });

    modelMatrix = glm::mat4();
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.6, 0.6, 0.6));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(120.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, 180.0f, glm::vec3(0.0f, -1.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    world.onHit(switchboard->createbox(modelMatrix), [&](Camera * c, SolidBox * b) {
        c->cancelMovement();
    });

    modelMatrix = glm::mat4();
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.75, 0.75, 0.75));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(20.0f, 0.0f, -10.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15, 0.15, 0.15));
    modelMatrix = glm::rotate(modelMatrix, -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    world.onHit(wagon->createbox(modelMatrix), [&](Camera * c, SolidBox * b) {
        c->cancelMovement();
    });

    modelMatrix = glm::mat4();
    modelMatrix = glm::scale(modelMatrix, glm::vec3(5.0, 6.0, 4.5));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0, 1.0, 0.0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0, 0.0, 5.0));
    modelMatrix = glm::rotate(modelMatrix, 90.0f, glm::vec3(0.0f, -1.0f, 0.0f));

    world.onHit(door.createbox(modelMatrix), [&](Camera * c, SolidBox * b) {
        if (show && !exiting) {
            hints.throwHint(EXIT_HINT, gamepad);
            torch.turnOff();
            indoorMusic.stop();
            doorSound.reset();
            doorSound.play();
            exiting = true;
            ((FirstPersonCamera *)c)->disable();
        }
    });

    modelMatrix = glm::mat4();
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2, 0.2, 0.2));
    modelMatrix = glm::rotate(modelMatrix , - 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, 90.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(100.0, 150.0, 0.0));

    world.onHit(bank->createbox(modelMatrix), [&](Camera * c, SolidBox * b) {
        c->cancelMovement();
    });

    modelMatrix = glm::mat4();
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5, 0.6, 0.5));
    modelMatrix = glm::rotate(modelMatrix ,- 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, 90.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-40, 75.0, 0.0));

    world.onHit(bed->createbox(modelMatrix), [&](Camera * c, SolidBox * b) {
        if (!showRocks) {
            hints.throwHint(ROCK_HINT, gamepad);
            player.setRockPickupState(true);
            lights[4].turnOff();
            showRocks = true;
            pickup2.reset();
            pickup2.play();
        }

        c->cancelMovement();
    });
}

void Indoors::onExit(int ID)
{
    FirstPersonCamera * camera = (FirstPersonCamera *)renderer3D.getCamera();
    camera->setPosition(glm::vec3(0.0, 13.0, -25.0));
    camera->setCameraDirection(0.0, -45.0);

    for (int i = 0; i < 5; i++) {	
        lights[i].turnOn();
    }

    torch.turnOff();

    showTorchLight = false;
    showRocks = false;
    exiting = false;
    show = false;
}