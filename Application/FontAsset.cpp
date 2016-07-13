
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

#include "FontAsset.h"

// Constructor & Deconstructor
FontAsset::FontAsset(std::string name)
{
	// Here we default the colour of the text to white
	colour = { 255, 255, 255 };

	// set the name and the text to be displayed
	this->name = name;
	text = name;
	font = NULL;
}

FontAsset::~FontAsset()
{
	if (font != NULL) {
		TTF_CloseFont(font);
	}
}

void FontAsset::recycle()
{

}

// This function generates a SDL Texture which can then be rendered on the screen
SDL_Surface * FontAsset::generateTexture()
{
	SDL_Surface * fontSurface = NULL;

	// if the font hasnt been loaded
	if (font == NULL || sz != -1) 
	{
		// load the font and set the size
		font = TTF_OpenFont(filename.c_str(), sz);
		sz = -1;
	}

	// when font has loaded
	if (font) 
	{
		// Load a blended surface
		fontSurface = TTF_RenderText_Blended(font, text.c_str(), colour);
		// then close the font
		TTF_CloseFont(font);
		// and reset the variable
		font = NULL;
	}

	// return the created surface even if NULL
	return fontSurface;
}

// This function sets the colour
void FontAsset::setColour(SDL_Color c) 
{
	colour = c;
}

// This function sets the text to be rendered
void FontAsset::setText(const char * t)
{
	text = t;
}

// This function sets the filename to load the font from
void FontAsset::grabFromFile(std::string font)
{
	filename = font;
}

// Here we set the size of the text
void FontAsset::openAtSize(int sz)
{
	this->sz = sz;
}

// here we just return the name of the object
std::string FontAsset::getName()
{
	return name;
}