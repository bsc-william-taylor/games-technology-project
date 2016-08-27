
#include "OptionsGraphics.h"

OptionsGraphics::OptionsGraphics() :
    package(nullptr),
    window(nullptr)
{
    colour = { 255, 255, 255 };
    currentSettings = MEDIUM;
}

OptionsGraphics::~OptionsGraphics()
{
}

void OptionsGraphics::create(LocalAssetManager * package, Window * window)
{
    this->package = package;
    this->window = window;

    graphicsSize.setFont(package->getL("data/fonts/Calibri", 60, colour ), "Game Graphics");
    graphicsSize.setArea(glm::vec2(650, 400), ALIGNMENT::CENTER);

    graphicsSizeButtons[0].setButtonTexture(package->getT("data/textures/left"));
    graphicsSizeButtons[0].setArea(glm::vec4(1250 - 300, 350, 70, 100), ALIGNMENT::CENTER);
    
    graphicsSizeButtons[1].setButtonTexture(package->getT("data/textures/right"));
    graphicsSizeButtons[1].setArea(glm::vec4(1250 + 250, 350, 70, 100), ALIGNMENT::CENTER);
}

void OptionsGraphics::onGamepadButton(int key, int state, int elementID)
{
    if (elementID == 3) 
    {
        colour = { 255, 0, 0 };

        if (key == SDL_CONTROLLER_BUTTON_DPAD_LEFT && state == GamepadButtonPressed) 
        {
            switch (currentSettings) 
            {
                case MEDIUM: 
                    currentSettings = LOW; 
                    break;
                case HIGH: 
                    currentSettings = MEDIUM; 
                    break;
                default: 
                    break;
            }
        }

        if (key == SDL_CONTROLLER_BUTTON_DPAD_RIGHT && state == GamepadButtonPressed) 
        {
            switch (currentSettings) 
            {
                case MEDIUM: 
                    currentSettings = HIGH; 
                    break;
                case LOW: 
                    currentSettings = MEDIUM; 
                    break;
                default: 
                    break;
            }
        }
    } 
    else 
    {
        colour = { 255, 255, 255 };
    }
}


void OptionsGraphics::render(Renderer2D * renderer)
{
    renderer->setAlpha(0.5);
    renderer->renderButtons(graphicsSizeButtons, 2);
    renderer->renderLabel(&graphicsSize);
    renderer->renderLabel(&graphicsizeLabel);
    renderer->setAlpha(1.0);
}

void OptionsGraphics::event(SDL_Event& e) 
{
    if (graphicsSizeButtons[0].isPressed(e))
    {
        switch (currentSettings) 
        {
            case MEDIUM: 
                currentSettings = LOW; 
                break;
            case HIGH: 
                currentSettings = MEDIUM; 
                break;

            default: break;
        }
    }

    if (graphicsSizeButtons[1].isPressed(e))
    {
        switch (currentSettings) 
        {
            case MEDIUM: 
                currentSettings = HIGH; 
                break;
            case LOW: 
                currentSettings = MEDIUM;
                break;

            default: break;
        }
    }
}

void OptionsGraphics::update()
{
    std::string setting = "";
    
    switch (this->currentSettings)
    {
        case MEDIUM: setting = "Medium"; break;
        case HIGH: setting = "High"; break;
        case LOW: setting = "Low"; break;
    default:
        break;
    }

    graphicsizeLabel.setFont(package->getL("data/fonts/Calibri", 60, colour ), setting.c_str());
    graphicsizeLabel.setArea(glm::vec2(1250, 400), ALIGNMENT::CENTER);
}