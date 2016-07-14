
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


#include "AssetManager.h"

// Constructor & Deconstructor 
AssetManager::AssetManager()
{
	stateAssetManagers.reserve(25);
	textures.reserve(250);
	models.reserve(250);
	fonts.reserve(250);
}

AssetManager::~AssetManager()
{
	for (auto& tex : textures)
		delete tex;
	for (auto& model : models)
		delete model;
	for (auto& font : fonts)
		delete font;
	for (auto& m : music)
		delete m;

	stateAssetManagers.clear();
}

void AssetManager::recycle()
{
	for (auto& tex : textures)
		tex->recycle();
}

std::vector<TextureAsset*>& AssetManager::getTextures()
{
	return textures;
}

std::vector<AudioAsset*>& AssetManager::getAudio()
{
	return this->music;
}

std::vector<ModelAsset*>& AssetManager::getModels()
{
	return models;
}

std::vector<FontAsset*>& AssetManager::getFonts()
{
	return fonts;
}

void AssetManager::push(TextureAsset * asset)
{
	textures.push_back(asset);
}

void AssetManager::push(ModelAsset * asset)
{
	models.push_back(asset);
}

void AssetManager::push(FontAsset * asset)
{
	fonts.push_back(asset);
}

void AssetManager::push(AudioAsset * asset)
{
	music.push_back(asset);
}

// Just returns a new asset manager for each state
LocalAssetManager * AssetManager::grabLocalManager()
{
	LocalAssetManager * localManager = new LocalAssetManager(this);
	this->stateAssetManagers.push_back(localManager);
	return localManager;
}

bool AssetManager::checkTexture(std::string ext, std::string nm)
{
	if (ext == ".jpg" || ext == ".png" || ext == ".bmp")
	{
		for (auto& texture : textures)
		{
			if (texture->getName() == nm)
			{
				//std::cout << "Duplicate Caught " << nm << std::endl;
				return false;
			}
		}

		return true;
	}

	return false;
}

bool AssetManager::checkModel(std::string ext, std::string nm)
{
	if (ext == ".3DS" || ext == ".obj" || ext == ".3ds" || ext == ".md2" || ext == ".md3")
	{
		for (auto& model : models)
		{
		
			if (model->getName() == nm)
			{
				//std::cout << "Duplicate Caught " << nm << std::endl;
				return false;
			}
		}

		return true;
	}

	return false;
}

bool AssetManager::checkLabel(std::string ext, std::string nm)
{
	if (ext == ".ttf" || ext == ".otf")
	{
		for (auto& label : fonts)
		{
			if (label->getName() == nm)
			{
				//std::cout << "Duplicate Caught " << nm << std::endl;
				return false;
			}
		}

		return true;
	}

	return false;
}