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
#include "TextureAsset.h"
#include "FontAsset.h"
#include "ModelFile.h"
#include "AudioFile.h"
#include "Media.h"

class AssetManager;

enum LOAD_COMMAND {
	DUPLICATE_FOUND,
	INVALID_FORMAT,
	NOT_FOUND
};

enum AUDIO_LOAD {
	STREAM = 1,
	LOAD = 0,
	STREAM_LOOP = 2,
	LOAD_LOOP = 3
};

// A class which loads assets for each application state
class LocalAssetManager
{
private:
	unsigned int referenceCount;
	AssetManager * assets;
public:
	// Constructor & Deconstructor
	LocalAssetManager(AssetManager *);
	~LocalAssetManager();
	
	// loads all the files given in the list and returns the number loaded
	unsigned int grab(std::initializer_list<std::string>);
	//
	AudioAsset * grabMusic(std::string filename, AUDIO_LOAD);

	// returns a texture object via filename (without extension)
	TextureAsset * getT(std::string filename);
	// returns a model object via filename (without extension)
	ModelAsset * getM(std::string filename);
	// returns a model object via filename (without extension)
	FontAsset * getL(std::string font, int, SDL_Color);
	// returns a audio asset via the filename
	AudioAsset * getA(std::string filename);

	void recycle();
private:
	// a list of all the filenames that were given
	std::vector<std::string> filenames;
};