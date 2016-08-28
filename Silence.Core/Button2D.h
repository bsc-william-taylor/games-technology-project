
#pragma once

#include "EngineLayer.h"
#include "Texture2D.h"
#include "Text2D.h"

class SILENCE_EXPORT Button2D
{
    Texture2D * buttonTexture;
    Text2D * buttonText;

    glm::vec4 buttonArea;
public:
    Button2D();
    ~Button2D();

    bool isPressed(SDL_Event&);

    void setButtonTexture(TextureAsset * texture);
    void setButtonText(FontAsset * font, const char * text);
    void setArea(glm::vec4, Alignment);
    void setArea(glm::vec4);
    
    Texture2D * getTexture();
    Text2D * getText();
};