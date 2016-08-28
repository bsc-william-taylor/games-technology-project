
#include "Hints.h"

Hints::Hints() :
    package(nullptr),
    hintText(" "), 
    alpha(0.0)
{					
}

Hints::~Hints()
{
}

void Hints::create(LocalAssetManager * package)
{
    this->package = package;

    hintRenderable.setFont(package->newFont("data/fonts/Calibri", 45, { 255, 255, 255 }), hintText.c_str());
    hintRenderable.setArea(glm::vec2(960, 1000), ALIGNMENT::CENTER);
}

void Hints::render(Renderer2D * renderer)
{
    renderer->setAlpha(alpha);
    renderer->renderLabel(&hintRenderable);
}

void Hints::throwHint(HintTypes hint, Gamepad * gamepad)
{
    switch (hint) 
    {
        case KeyHint: 
        {
            hintText = "This may not be the only key you need to find..."; 
            break;
        }

        case ExitHint: 
        {
            hintText = "Good luck..."; 
            break;
        }

        case RockHint: 
        {
            if (gamepad->isConnected())
            {
                hintText = "Throw rocks to distract your blind opponent with the right bumper...";
            }
            else
            {
                hintText = "Throw rocks to distract your blind opponent with the E key...";
            }

            break;
        }

        case TorchHint: 
        {
            if (gamepad->isConnected())
            {
                hintText = "Press X to toggle... its awfully dark outside...";
            }
            else
            {
                hintText = "Press F to toggle... its awfully dark outside...";
            }

            break;
        }

        default:
            break;
    }

    alpha = 1.0;
}

void Hints::update()
{
    alpha -= 0.005;

    if ("Good luck..." == hintText) 
    {
        hintRenderable.setFont(package->newFont("data/fonts/Calibri", 70, { 255, 0, 0 }), hintText.c_str());
        hintRenderable.setArea(vec2(960, 1080/2), ALIGNMENT::CENTER);
    } 
    else 
    {
        hintRenderable.setFont(package->newFont("data/fonts/Calibri", 45, { 255, 255, 255 }), hintText.c_str());
        hintRenderable.setArea(vec2(960, 1000), ALIGNMENT::CENTER);
    }
}