
#include "Background.h"

Background::Background() :
    package(nullptr),
    alpha(1.0f),
    index(0)
{
}

Background::~Background()
{
}

void Background::create(LocalAssetManager * package)
{
    this->package = package;

    background.setTexture(package->getT("data/textures/menu1"));
    background.setArea(glm::vec4(0.0, 0.0, 1920, 1080));
}

void Background::render(Renderer2D * renderer)
{
    renderer->renderTexture(&background);
}

void Background::enter()
{
    timer.start();
}

float Background::getAlpha()
{
    return alpha;
}

void Background::event(SDL_Event&)
{
}

void Background::update()
{
    if (alpha + 0.01f < 1.0f)
    {
        alpha += 0.01f;
    }

    if (timer.elapsed(SECONDS) >= 10)
    {
        alpha = 0.0f;

        switch (++index)
        {
            case 0: background.setTexture(package->getT("data/textures/menu1")); break;
            case 1: background.setTexture(package->getT("data/textures/menu2")); break;
            case 2: background.setTexture(package->getT("data/textures/menu3")); break;

            default: background.setTexture(package->getT("data/textures/menu1")); index = 0; break;
        }

        timer.clear();
        timer.start();
    }
}
