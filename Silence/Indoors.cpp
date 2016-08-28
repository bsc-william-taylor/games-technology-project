
#include "Indoors.h"
#include "Silence.h"

Indoors::Indoors(OperatingSystem * engine) :
    switchboard(nullptr),
    generator(nullptr),
    monster(nullptr),
    wagon(nullptr),
    bank(nullptr),
    bed(nullptr),  
    alpha(0.0)
{
    manager = engine->acquireSceneManager();
    window = &engine->acquireWindow();
    gamepad = engine->acquireGamepad();

    package = engine->acquireAssetManager()->grabLocalManager();
    package->grab({ 
        "data/textures/brick.png",
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
    SAFE_RELEASE(switchboard);
    SAFE_RELEASE(generator);
    SAFE_RELEASE(wagon);
    SAFE_RELEASE(bank);
    SAFE_RELEASE(bed); 
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
        if (showTorchLight) 
        {
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

void Indoors::onCreate()
{
    player.create(package);
    
    cube.setArea(vec3(-100, 0.0, -50), vec3(200.0, 0.25, 100.0), 20);
    cube.setTexture(package->newTexture("data/textures/brick"));
    model.setModel(package->newModel("data/models/house/br_house1"));
    body.setModel(package->newModel("data/models/hang/hang2"));
    door.setModel(package->newModel("data/models/door/door"));
    key.setModel(package->newModel("data/models/key/key"));
    rocks.setModel(package->newModel("data/models/rocks/obj"));
    torchLight.setModel(package->newModel("data/models/torchlight/Linterna"));
    matrices.makeProjection3D(60.0f, glm::vec2(0.1f, 1000.0f));

    renderer3D.createRenderer();
    hints.create(package);

    renderer3D.changeCamera(CameraType::FirstPerson);
    renderer3D.setCameraArea(glm::vec4(-75.0f, -25.0f, 75.0, 25.0f));
    renderer3D.setCameraPosition(glm::vec3(0.0, 13.0, -25.0));

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

void Indoors::onGameEvent(SDL_Event& e)
{
    renderer3D.updateCamera(e);

    if (e.key.keysym.sym == SDLK_f && e.type == SDL_KEYUP)
    {
        if (showTorchLight) 
        {
            torchSound.reset();
            torchSound.play();
            torch.toggle();
        }
    }
}

void Indoors::onUpdate()
{
    auto camera = static_cast<FirstPersonCamera *>(renderer3D.getCamera());
    player.update(camera, nullptr);

    if (exiting) 
    {
        alpha -= 0.004f;
        if (alpha <= -0.1f) 
        {
            manager->switchScene(int(SceneID::Forest));
        }
    } 
    else 
    {
        alpha = 1.0f;
    }

    camera->prepareCamera();

    world.updateWorld();
    hints.update();

    renderer3D.repositionCamera();
}

void Indoors::onEnter(int)
{
    auto camera = static_cast<FirstPersonCamera *>(renderer3D.getCamera());
    camera->enable();

    SDL_SetRelativeMouseMode(SDL_TRUE);

    indoorMusic.play();
    jumpSound.reset();
    jumpSound.play();
    show = false;
}

void Indoors::onRender()
{
    renderer3D.prepare();
    renderer3D.setProjectionMatrix(matrices);

    matrices.push();
    matrices.scale(vec3(2.5, 2.0, 2.0));
    matrices.translate(vec3(0.0, -1.0, 0.0));
    matrices.rotate(-90.0f, vec3(1.0f, 0.0f, 0.0f));

    renderer3D.setAlpha(alpha);
    renderer3D.setModelMatrix(matrices);
    renderer3D.renderModel(&model);

    matrices.pop();

    float angles[] = { 45.0f, 90.0f, 155.0f };
    float xs[] = { -10.0f, 10.0f, -12.0f };
    float zs[] = { 30.0f, -10.0f, -50.0f };

    for (auto i = 0; i < 3; i++)
    {
        matrices.push();
        matrices.scale(vec3(0.3, 0.3, 0.3));
        matrices.rotate(angles[i], vec3(0.0, 1.0, 0.0));
        matrices.translate(vec3(xs[i], 60, zs[i]));

        renderer3D.setModelMatrix(matrices);
        renderer3D.renderModel(&body);

        matrices.pop();
    }

    if (!show)
    {
        matrices.push();
        matrices.translate(vec3(30.0f, 0.4f, -9.0));
        matrices.rotate(-90.0f, vec3(1.0f, 0.0f, 0.0f));
        matrices.scale(vec3(0.5f, 0.5, 0.5f));

        
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

    if (!showTorchLight) 
    {
        matrices.push();
        matrices.translate(glm::vec3(-25.0, 8.25, -20.0));
        matrices.scale(glm::vec3(0.5, 0.5, 0.5));

        renderer3D.setModelMatrix(matrices);
        renderer3D.renderModel(&torchLight);

        matrices.pop();
    }

    if (!showRocks) 
    {
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

    auto projectionMatrix = glm::mat4();
    auto modelMatrix = glm::mat4();

    projectionMatrix = glm::ortho(0.0, 1920.0, 0.0, 1080.0, -1.0, 1.0);

    renderer2D.prepare();
    renderer2D.setMatrixForObject("projection", projectionMatrix);
    renderer2D.setMatrixForObject("model", modelMatrix);

    hints.render(&renderer2D);

    renderer2D.present();
}

void Indoors::generateRandomLights()
{
    float power[] = { 0.0008f, 0.0008f, 0.0016f, 0.0016f, 0.0016f };
    int xpos[] = { -50, 50, 30, -30, -40 };
    int zpos[] = { 0, 0, -8, -16, 20 };

    vec3 colours[] = {
        vec3(1.0, 0.0, 0.0),
        vec3(1.0, 0.0, 0.0),
        vec3(0.3, 0.25, 0),
        vec3(0.3, 0.25, 0),
        vec3(0.3, 0.25, 0)
    };

    const auto sceneLights = renderer3D.getLights();
    sun.setDirection(glm::vec3(0.0, -1.0, 0.3));
    sun.setColour(glm::vec3(0.5, 0.25, 0.25));
    sun.turnOn();

    for (auto i = 0; i < 5; i++)
    {
        lights[i].setAttribuation(glm::vec4(0.1f, 0.3f, 0.007f, power[i]));
        lights[i].setPosition(vec3(xpos[i], i >= 3 ? 14.0f : 0.0f, zpos[i]));
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
    auto modelMatrix = glm::mat4();
    modelMatrix = glm::translate(modelMatrix, vec3(-70.0f, 0.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, vec3(0.1, 0.1, 0.1));
    modelMatrix = glm::rotate(modelMatrix, -90.0f, vec3(1.0f, 0.0f, 0.0f));

    world.setPlayerCamera(renderer3D.getCamera());
    world.onHit(generator->createbox(modelMatrix), [&](Camera * c, SolidBox * b) { c->cancelMovement(); });

    modelMatrix = glm::mat4();
    modelMatrix = glm::scale(modelMatrix, vec3(0.2, 0.2, 0.2));
    modelMatrix = glm::rotate(modelMatrix, -90.0f, vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, 90.0f, vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::translate(modelMatrix, vec3(100.0, 150.0, 0.0));

    world.onHit(bank->createbox(modelMatrix), [&](Camera * c, SolidBox * b) {
        if (!showTorchLight) {
            hints.throwHint(TorchHint, gamepad);

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
    modelMatrix = glm::translate(modelMatrix, vec3(30.0f, 0.4f, -9.0));
    modelMatrix = glm::rotate(modelMatrix, -90.0f, vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, vec3(2.0f, 2.0f, 2.0f));
    
    world.onHit(key.createbox(modelMatrix), [&](Camera * c, SolidBox * b) {
        if (!show) {
            hints.throwHint(KeyHint, gamepad);
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

    world.onHit(switchboard->createbox(modelMatrix), [&](Camera * c, SolidBox * b) { c->cancelMovement(); });

    modelMatrix = glm::mat4();
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.75, 0.75, 0.75));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(20.0f, 0.0f, -10.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15, 0.15, 0.15));
    modelMatrix = glm::rotate(modelMatrix, -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    world.onHit(wagon->createbox(modelMatrix), [&](Camera * c, SolidBox * b) { c->cancelMovement(); });

    modelMatrix = glm::mat4();
    modelMatrix = glm::scale(modelMatrix, vec3(5.0, 6.0, 4.5));
    modelMatrix = glm::translate(modelMatrix, vec3(0.0, 1.0, 0.0));
    modelMatrix = glm::translate(modelMatrix, vec3(0.0, 0.0, 5.0));
    modelMatrix = glm::rotate(modelMatrix, 90.0f, vec3(0.0f, -1.0f, 0.0f));

    world.onHit(door.createbox(modelMatrix), [&](Camera * c, SolidBox * b) {
        if (show && !exiting) {
            const auto camera = static_cast<FirstPersonCamera *>(c);
            hints.throwHint(ExitHint, gamepad);
            torch.turnOff();
            indoorMusic.stop();
            doorSound.reset();
            doorSound.play();
            exiting = true;
            camera->disable();
        }
    });

    modelMatrix = glm::mat4();
    modelMatrix = glm::scale(modelMatrix, vec3(0.2, 0.2, 0.2));
    modelMatrix = glm::rotate(modelMatrix , - 90.0f, vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, 90.0f, vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::translate(modelMatrix, vec3(100.0, 150.0, 0.0));

    world.onHit(bank->createbox(modelMatrix), [&](Camera * c, SolidBox * b) { c->cancelMovement(); });

    modelMatrix = glm::mat4();
    modelMatrix = glm::scale(modelMatrix, vec3(0.5, 0.6, 0.5));
    modelMatrix = glm::rotate(modelMatrix ,- 90.0f, vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, 90.0f, vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::translate(modelMatrix, vec3(-40, 75.0, 0.0));

    world.onHit(bed->createbox(modelMatrix), [&](Camera * c, SolidBox * b) {
        if (!showRocks) {
            hints.throwHint(RockHint, gamepad);
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
    const auto camera = static_cast<FirstPersonCamera *>(renderer3D.getCamera());
    camera->setPosition(glm::vec3(0.0, 13.0, -25.0));
    camera->setCameraDirection(0.0, -45.0);

    for(auto& light : lights)
    {
        light.turnOn();
    }

    torch.turnOff();

    showTorchLight = false;
    showRocks = false;
    exiting = false;
    show = false;
}