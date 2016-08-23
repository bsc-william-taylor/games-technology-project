
#include "Headers.h"

Headers::Headers() :
    package(nullptr)
{
}

Headers::~Headers()
{
}

void Headers::create(LocalAssetManager * package)
{
    this->package = package;

    header.setFont(package->getL("data/fonts/Calibri", 155, { 255, 255, 255 }), "Silence : Enter the Forest");
    header.setArea(glm::vec2(1920 / 2, 900), ALIGNMENT::CENTER);

    footer.setFont(package->getL("data/fonts/Calibri", 25, { 255, 255, 255 }), "'You may want to scream, you may want to cry for help but if you want to live and escape the clutches of the forest the key to your survival is... Silence'");
    footer.setArea(glm::vec2(1920 / 2, 100), ALIGNMENT::CENTER);
}

void Headers::render(Renderer2D * renderer)
{
    renderer->renderLabel(&header);
    renderer->renderLabel(&footer);
}

void Headers::event(SDL_Event&)
{
}

void Headers::update()
{
}

void Headers::enter()
{
}