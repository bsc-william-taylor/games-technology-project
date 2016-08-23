
#include "OptionsField.h"

OptionsField::OptionsField(Window * window) :
    optionsWindowTypeActor(window),
    optionsWindowSizeActor(window),
    package(nullptr),
    window(window),
    active(0)
{
}

OptionsField::~OptionsField()
{
}

void OptionsField::onGamepadButton(int key, int state)
{
    if (key == SDL_CONTROLLER_BUTTON_DPAD_RIGHT && active == 0 && state == GAMEPAD_BUTTON_PRESSED)
    {
        ++active;
    }

    if (key == SDL_CONTROLLER_BUTTON_DPAD_LEFT && active == 1 && state == GAMEPAD_BUTTON_PRESSED)
    {
        --active;
    }

    if (key == SDL_CONTROLLER_BUTTON_DPAD_DOWN && state == GAMEPAD_BUTTON_PRESSED) 
    {
        if (active == 4)
        {
            active = 0;
        }
        else
        {
            active <= 1 ? active = 2 : active++;
        }
    }

    if (key == SDL_CONTROLLER_BUTTON_DPAD_UP && state == GAMEPAD_BUTTON_PRESSED) 
    {
        if (active == 0)
        {
            active = 4;
        }
        else
        {
            active == 2 ? active = 0 : active--;
        }
    }

    optionsWindowTypeActor.onGamepadButton(key, state, active);
    optionsWindowSizeActor.onGamepadButton(key, state, active);
    //optionsGraphicsActor->onGamepadButton(key, state, active);
    optionsVolumeActor.onGamepadButton(key, state, active);
}

void OptionsField::onGameEvent(SDL_Event& e)
{
    optionsWindowTypeActor.event(e);
    optionsWindowSizeActor.event(e);
    //optionsGraphicsActor->event(e);
    optionsVolumeActor.event(e);
}

void OptionsField::render(Renderer2D * renderer)
{
    optionsWindowSizeActor.render(renderer);
    optionsWindowTypeActor.render(renderer);
    optionsGraphicsActor.render(renderer);
    optionsVolumeActor.render(renderer);
}

void OptionsField::create(LocalAssetManager * package)
{
    this->package = package;

    optionsWindowTypeActor.create(package);
    optionsWindowSizeActor.create(package);
    optionsGraphicsActor.create(package, this->window);
    optionsVolumeActor.create(package);

    optionsWindowTypeActor.makeActive(active);
}

void OptionsField::update()
{
    optionsWindowSizeActor.update();
    optionsGraphicsActor.update();
    optionsVolumeActor.update();
}