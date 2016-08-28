
#include "OptionsWindowType.h"

OptionsWindowType::OptionsWindowType(Window * window) :
    package(nullptr),
    window(window)
{
    colour1 = { 255, 255, 255 };
    colour2 = { 255, 255, 255 };
}

OptionsWindowType::~OptionsWindowType()
{
}

void OptionsWindowType::render(Renderer2D * renderer)
{
    renderer->renderButtons(windowModes, 2);
    renderer->renderLabel(&windowMode);
}

void OptionsWindowType::makeActive(unsigned int elementActiveID)
{
    if (elementActiveID == 0)
    {
        colour2 = { 255, 255, 255 };
        colour1 = { 255, 0, 0 };
    }
    else if (elementActiveID == 1)
    {
        colour1 = { 255, 255, 255};
        colour2 = { 255, 0, 0 };
    }
    else 
    {
        colour1 = { 255, 255, 255 };
        colour2 = { 255, 255, 255 };
    }
    
    windowModes[0].setButtonText(package->newFont("data/fonts/Calibri", 40, colour1), "Windowed");
    windowModes[1].setButtonText(package->newFont("data/fonts/Calibri", 40, colour2 ), "Fullscreen");
}

void OptionsWindowType::onGamepadButton(int key, int state, int e)
{
    if (e == 0 && key == SDL_CONTROLLER_BUTTON_A && state == GamepadButtonPressed)
    {
        window->forceWindowedMode();
        window->adjustViewport();
    }

    if (e == 1 && key == SDL_CONTROLLER_BUTTON_A && state == GamepadButtonPressed)
    {
        window->forceFullscreenMode();
        window->adjustViewport();
    }
    
    makeActive(e);
}

void OptionsWindowType::create(LocalAssetManager * package)
{
    this->package = package;

    windowMode.setFont(package->newFont("data/fonts/Calibri", 60, { 255, 255, 255 }), "Window Mode");
    windowMode.setArea(glm::vec2(650, 700), Alignment::Center);
    
    windowModes[0].setButtonTexture(package->newTexture("data/textures/button"));
    windowModes[0].setButtonText(package->newFont("data/fonts/Calibri", 40, { 255, 255, 255 }), "Windowed");
    windowModes[0].setArea(glm::vec4(950, 600, 300, 200), Alignment::Center);
    
    windowModes[1].setButtonTexture(package->newTexture("data/textures/button"));
    windowModes[1].setButtonText(package->newFont("data/fonts/Calibri", 40, { 255, 255, 255 }), "Fullscreen");
    windowModes[1].setArea(glm::vec4(1250, 600, 300, 200), Alignment::Center);
}

void OptionsWindowType::event(SDL_Event& e)
{
    if (windowModes[0].isPressed(e))
    {
        window->forceWindowedMode();
        window->adjustViewport();
    }

    if (windowModes[1].isPressed(e))
    {
        window->forceFullscreenMode();
        window->adjustViewport();
    }
}