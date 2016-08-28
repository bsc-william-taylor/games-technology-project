
#include "Silence.h"

Options::Options(OperatingSystem * system)
{
    scenes = system->acquireSceneManager();
    package = system->acquireAssetManager()->grabLocalManager();
    package->grab({ "data/textures/options.png",
        "data/fonts/Calibri.ttf",
        "data/textures/button.png",
        "data/textures/back.png",
        "data/textures/right.png",
        "data/textures/left.png"
    });

    optionInputs = new OptionsField(&system->acquireWindow());
}

Options::~Options()
{
    SAFE_RELEASE(optionInputs);
    SAFE_RELEASE(package);
}

void Options::onCreate()
{
    optionInputs->create(package);

    header.setFont(package->newFont("data/fonts/Calibri", 225, { 255, 255, 255 }), "Options");
    header.setArea(glm::vec2(1920 / 2, 900), Alignment::Center);

    version.setFont(package->newFont("data/fonts/Calibri", 25, { 255, 255, 255 }), "Version 1.0");
    version.setArea(glm::vec2(1920 / 2, 100), Alignment::Center);

    backButton.setButtonTexture(package->newTexture("data/textures/back"));
    backButton.setArea(glm::vec4(10, 945, 125, 125), Alignment::Right);

    background.setTexture(package->newTexture("data/textures/options"));
    background.setArea(glm::vec4(0.0, 0.0, 1920, 1080));

    renderer2D.createRenderer();
}

void Options::onGamepadButton(int key, int state)
{
    if ((key == SDL_CONTROLLER_BUTTON_B || key == SDL_CONTROLLER_BUTTON_BACK) && state == GamepadButtonPressed) 
    {
        scenes->switchScene(static_cast<int>(SceneID::Menu));
    }

    optionInputs->onGamepadButton(key, state);	
}

void Options::onGameEvent(SDL_Event& e)
{
    optionInputs->onGameEvent(e);

    if (backButton.isPressed(e))
    {
        scenes->switchScene(static_cast<int>(SceneID::Menu));
    }
}

void Options::onUpdate()
{
    optionInputs->update();
}

void Options::onRender()
{
    auto projectionMatrix = glm::mat4();
    auto modelMatrix = glm::mat4();

    projectionMatrix = glm::ortho(0.0, 1920.0, 0.0, 1080.0, -1.0, 1.0);

    renderer2D.clear();
    renderer2D.prepare();
    renderer2D.setMatrixForObject("projection", projectionMatrix);
    renderer2D.setMatrixForObject("model", modelMatrix);
    renderer2D.setAlpha(1.0f);
    renderer2D.renderTexture(&background);
    renderer2D.renderButton(&backButton);

    optionInputs->render(&renderer2D);
    
    renderer2D.renderLabel(&version);
    renderer2D.renderLabel(&header);
    renderer2D.present();
}