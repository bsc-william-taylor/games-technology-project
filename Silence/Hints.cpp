
#include "Hints.h"

Hints::Hints()
{																	
    this->hintText = " ";
    alpha = 0.0;
}

Hints::~Hints()
{

}

void Hints::create(LocalAssetManager * package)
{
    this->package = package;

    hintRenderable.setFont(package->getL("data/fonts/Calibri", 45, { 255, 255, 255 }), this->hintText.c_str());
    hintRenderable.setArea(glm::vec2(960, 1000), ALIGNMENT::CENTER);
}

void Hints::render(Renderer2D * renderer)
{
    renderer->setAlpha(alpha);
    renderer->renderLabel(&hintRenderable);
}

void Hints::throwHint(HINT_TYPES hint, Gamepad * gamepad)
{
    switch (hint) {
        case KEY_HINT: this->hintText = "This may not be the only key you need to find..."; break;
        case EXIT_HINT: this->hintText = "Good luck..."; break;

        case ROCK_HINT: {
            if (gamepad->isConnected())
                this->hintText = "Throw rocks to distract your blind opponent with the right bumper...";
            else
                this->hintText = "Throw rocks to distract your blind opponent with the E key...";
            break;
        }

        case TORCH_HINT: {
            if (gamepad->isConnected())
                this->hintText = "Press X to toggle... its awfully dark outside...";
            else
                this->hintText = "Press F to toggle... its awfully dark outside...";
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

    if ("Good luck..." == this->hintText) {
        hintRenderable.setFont(this->package->getL("data/fonts/Calibri", 70, { 255, 0, 0 }), this->hintText.c_str());
        hintRenderable.setArea(glm::vec2(960, 1080/2), ALIGNMENT::CENTER);
    } else {
        hintRenderable.setFont(this->package->getL("data/fonts/Calibri", 45, { 255, 255, 255 }), this->hintText.c_str());
        hintRenderable.setArea(glm::vec2(960, 1000), ALIGNMENT::CENTER);
    }
}