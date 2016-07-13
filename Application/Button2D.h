
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

#pragma once

#include "EngineLayer.h"
#include "Texture2D.h"
#include "Text2D.h"

// A button object which sets up both a text and texture object to represent a button
class Button2D 
{
private:
	// The texture for the button
	Texture2D * buttonTexture;
	// The text for the button
	Text2D * buttonText;
	// The buttons area in 2D space
	glm::vec4 buttonArea;
public:
	// Constructor & Deconstructor
	Button2D();
	~Button2D();

	// This function sets the background texture for the button
	void setButtonTexture(TextureAsset * texture);
	// This function sets the text for the button
	void setButtonText(FontAsset * font, const char * text);
	// This function sets the area the button will be rendered in and the alignment of the text
	void setArea(glm::vec4, ALIGNMENT);
	// // This function sets the area the button
	void setArea(glm::vec4);
	// This function just checks a system event to see if the button was pressed
	bool isPressed(SDL_Event&);
	
	// This function returns a pointer to the buttons texture object
	Texture2D * getTexture();
	// This function returns a pointer to the buttons text object
	Text2D * getText();
};