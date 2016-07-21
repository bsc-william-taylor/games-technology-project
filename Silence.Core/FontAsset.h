
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

// a class which handles loading a TTF_Font object
class SILENCE_EXPORT FontAsset
{
private:
    // the colour for the text
    SDL_Color colour;
    // a pointer to the font being used
    TTF_Font * font;
    // the size of the font to be user
    unsigned int sz;
public:
    // Constructor & Deconstructor
    FontAsset(std::string name);
    ~FontAsset();

    // returns a surface with the text written on it
    SDL_Surface * generateTexture();

    // grabs and loads the TTF || OTF file
    void grabFromFile(std::string);
    // sets the colour of the font
    void setColour(SDL_Color c);
    // sets the text to be rendered into the surface
    void setText(const char *);
    // sets the size of the text being rendered
    void openAtSize(int);
    //
    void recycle();

    // returns the name of the object
    std::string getName();
private:
    // the place it was loaded from
    std::string filename;
    // the text to be rendered
    std::string text;
    // and the name for this asset
    std::string name;
};