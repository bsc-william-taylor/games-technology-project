
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

class SILENCE_EXPORT TextureAsset
{
private:
	// the surface with the loaded pixels
	FIBITMAP * bitmap;
	// the name of the asset
	std::string name;
public:
	// Constructor & Deconstructor
	TextureAsset(std::string name);
	~TextureAsset();
	
	// returns the name of the object (Used by asset manager to ID a resource to return)
	std::string getName();

	// opens and loads the file given by filename
	void grabFromFile(const char * filename);
	// returns a void pointer to the pixels
	void* getPixels();
	void recycle();

	RGBQUAD getPixelColour(int x, int y);
	// here we just returns stats about the surface loaded
	int getHeight(); // returns height
	int getWidth(); // returns width
	int getMask(); // returns format 
	int getBPP(); // returns bits per pixel
};