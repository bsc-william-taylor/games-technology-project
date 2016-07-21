
/**
*
* Copyright (c) 2014 : William Taylor : wi11berto@yahoo.co.uk
*
* This software is provided 'as-is', without any
* express or implied warranty. In no event will
* the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute
* it freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented;
*    you must not claim that you wrote the original software.
*    If you use this software in a product, an acknowledgment
*    in the product documentation would be appreciated but
*    is not required.
*
* 2. Altered source versions must be plainly marked as such,
*    and must not be misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source distribution.
*/

#include "Button2D.h"

// Constructor & Deconstructor
Button2D::Button2D()
    : buttonTexture(NULL),
      buttonText(NULL)
{
}

Button2D::~Button2D()
{
    SAFE_RELEASE(buttonTexture);
    SAFE_RELEASE(buttonText);
}

// This function just sets the buttons texture
void Button2D::setButtonTexture(TextureAsset * text)
{
    // make sure we only load once
    if (buttonTexture == NULL)
    {
        // create the texture object and then call its setTexture method
        buttonTexture = new Texture2D();
    }

    buttonTexture->setTexture(text);
}

// This function just sets the buttons texture
void Button2D::setButtonText(FontAsset * f, const char * text)
{
    // make sure we only load once
    if (buttonText == NULL)
    {	
        // create the text object and then call its setFont method
        buttonText = new Text2D();
    }

    buttonText->setFont(f, text);
}

// This function sets the area that the button will be rendered in 2D space
void Button2D::setArea(glm::vec4 vec)
{
    // Set the area of the texture if its been loaded
    if (buttonTexture)
    {
        buttonTexture->setArea(vec);
    }

    // Set the area of the text if its been loaded
    if (buttonText)
    {
        buttonText->setArea(glm::vec2(vec[0], vec[1]));
    }
}

// This function sets the area the button will be rendered into
void Button2D::setArea(glm::vec4 vec, ALIGNMENT alignment)
{
    // create a position vector
    glm::vec2 position(vec[0], vec[1]);

    // and calculate the position based on the alignment
    switch (alignment)
    {
        // Should implement LEFT & RIGHT Alignment later
        case ALIGNMENT::CENTER:
        {
            position[0] = vec[0] + vec[2]/2;
            position[1] = vec[1] + vec[3]/2;
            break;
        }
        
    default:
        break;
    }

    // then set the area of the texture if its been loaded
    if (buttonTexture)
    {
        buttonTexture->setArea(vec);
    }

    // then set the area of the text if its been loaded
    if (buttonText)
    {
        buttonText->setArea(position, alignment);
    }
    
    // and set the area variable
    buttonArea = vec;
}

// This function checks to see if the button was pressed and returns true if it has been pressed
bool Button2D::isPressed(SDL_Event& e) 
{
    // check if event passed is a mouse event
    if (e.button.state == SDL_PRESSED && e.button.button == SDL_BUTTON_LEFT) 
    {
        // get the mouses position in 2D space
        int mx = e.motion.x;
        int my = e.motion.y;

        // then calculate the position
        SDL_Rect pos = {
            buttonArea[0], buttonArea[1],
            buttonArea[2], buttonArea[3]
        };

        // and check to see if its inside the button
        return(mx >= pos.x && mx <= pos.x + pos.w && my >= pos.y && my <= pos.y + pos.h);
    }	

    return false;
}

// returns the texture object as a pointer
Texture2D * Button2D::getTexture()
{
    return buttonTexture;
}

// returns the text object as a pointer
Text2D * Button2D::getText()
{
    return buttonText;
}