
#include "Buttons.h"

Buttons::Buttons() :
    package(nullptr),
    active(0)
{
}

Buttons::~Buttons()
{
}

void Buttons::create(LocalAssetManager * package)
{
    roarSound.open(package->newAudio("data/media/roar.mp3", Load));
    this->package = package;
    load(active);
}

void Buttons::load(int active)
{
    std::string texts[] = { "Play Game", "Options", "Quit" };
    auto y = 550;

    for (auto i = 0; i < 3; i++) 
    {
        buttons[i].setButtonTexture(package->newTexture("data/textures/button"));

        if (active == i)
        {
            buttons[i].setButtonText(package->newFont("data/fonts/Calibri", 40, { 255, 0, 0 }), texts[i].c_str());
        }
        else
        {
            buttons[i].setButtonText(package->newFont("data/fonts/Calibri", 40, { 255, 255, 255 }), texts[i].c_str());
        }

        buttons[i].setArea(glm::vec4(300, y, 400, 225), Alignment::Center);
        y -= 150;
    }
}

void Buttons::onGamepadButton(int button, int state)
{
    if (button == SDL_CONTROLLER_BUTTON_DPAD_DOWN && state == GamepadButtonPressed)
    {
        if (active == 2) 
        {
            active = 0;
        } 
        else 
        {
            active++;
        }

        load(active);
    }

    if (button == SDL_CONTROLLER_BUTTON_DPAD_UP && state == GamepadButtonPressed)
    {
        if (active == 0) 
        {
            active = 2;
        } 
        else 
        {
            active--;
        }

        load(active);
    }
}

bool Buttons::isPressed(int button, int i, int s)
{
    if (button == active && i == SDL_CONTROLLER_BUTTON_A && s == GamepadButtonPressed)
    {
        if (button == NULL) 
        {
            roarSound.play();
        }

        return true;
    }

    return false;
}

void Buttons::render(Renderer2D * renderer)
{
    renderer->renderButtons(buttons, 3);
}

void Buttons::event(SDL_Event&)
{
}

void Buttons::update()
{
}

bool Buttons::isPressed(int i, SDL_Event& e)
{
    return buttons[i].isPressed(e);
}

void Buttons::enter()
{
}