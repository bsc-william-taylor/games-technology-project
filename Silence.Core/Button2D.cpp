

#include "Button2D.h"

Button2D::Button2D()
    : buttonTexture(nullptr),
      buttonText(nullptr)
{
}

Button2D::~Button2D()
{
    SAFE_RELEASE(buttonTexture);
    SAFE_RELEASE(buttonText);
}

void Button2D::setButtonTexture(TextureAsset * text)
{
    if (buttonTexture == nullptr)
    {
        buttonTexture = new Texture2D();
    }

    buttonTexture->setTexture(text);
}

void Button2D::setButtonText(FontAsset * f, const char * text)
{
    if (buttonText == nullptr)
    {	
        buttonText = new Text2D();
    }

    buttonText->setFont(f, text);
}

void Button2D::setArea(glm::vec4 vec)
{
    if (buttonTexture)
    {
        buttonTexture->setArea(vec);
    }

    if (buttonText)
    {
        buttonText->setArea(glm::vec2(vec[0], vec[1]));
    }
}

void Button2D::setArea(glm::vec4 vec, Alignment alignment)
{
    glm::vec2 position(vec[0], vec[1]);

    switch (alignment)
    {
        case Alignment::Center:
        {
            position[0] = vec[0] + vec[2]/2;
            position[1] = vec[1] + vec[3]/2;
            break;
        }
        
    default:
        break;
    }

    if (buttonTexture)
    {
        buttonTexture->setArea(vec);
    }

    if (buttonText)
    {
        buttonText->setArea(position, alignment);
    }
    
    buttonArea = vec;
}

bool Button2D::isPressed(SDL_Event& e) 
{
    if (e.button.state == SDL_PRESSED && e.button.button == SDL_BUTTON_LEFT) 
    {
        auto mx = e.motion.x;
        auto my = e.motion.y;

        SDL_Rect pos = {
            buttonArea[0], buttonArea[1],
            buttonArea[2], buttonArea[3]
        };

        return(mx >= pos.x && mx <= pos.x + pos.w && my >= pos.y && my <= pos.y + pos.h);
    }	

    return false;
}

Texture2D * Button2D::getTexture()
{
    return buttonTexture;
}

Text2D * Button2D::getText()
{
    return buttonText;
}