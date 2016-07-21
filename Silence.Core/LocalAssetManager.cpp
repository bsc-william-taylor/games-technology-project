
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


#include "LocalAssetManager.h"
#include "AssetManager.h"

// Constructor & Decosntructor
LocalAssetManager::LocalAssetManager(AssetManager * assets)
{
    this->assets = assets;
}

LocalAssetManager::~LocalAssetManager()
{
}

// looks for the texture object specified via the filename
TextureAsset * LocalAssetManager::getT(std::string filename)
{
    auto textures = assets->getTextures();

    for (auto& tex : textures) {
        if (tex->getName() == filename) {
            return tex;
        }
    }

    return NULL;
}

//
void LocalAssetManager::recycle()
{
    assets->recycle();
}

//
AudioAsset * LocalAssetManager::grabMusic(std::string filename, AUDIO_LOAD load)
{
    AudioAsset * audio = new AudioAsset(filename.substr(0, filename.length() - 4));

    switch (load) {
        case STREAM_LOOP: audio->grabFromFile(filename, true, true); break;
        case LOAD_LOOP: audio->grabFromFile(filename, false, true); break;
        case STREAM: audio->grabFromFile(filename, true, false); break;
        case LOAD: audio->grabFromFile(filename, false, false); break;

        default: break;
    };

    assets->push(audio);
    return audio;
}

// looks for the model object specified via the filename
ModelAsset * LocalAssetManager::getM(std::string filename)
{
    auto models = assets->getModels();

    for (auto& model : models) {
        if (model->getName() == filename) {
            return model;
        }
    }
    
    return NULL;
}

//
AudioAsset * LocalAssetManager::getA(std::string filename)
{
    auto audioFiles = assets->getAudio();

    for (auto& audio : audioFiles) {
        if (audio->getName() == filename) {
            return audio;
        }
    }

    return NULL;
}


// looks for the label object specified via the filename
FontAsset * LocalAssetManager::getL(std::string font, int sz, SDL_Color c)
{
    auto fonts = assets->getFonts();

    for (auto& label : fonts) {
        if (label->getName() == font) {
            label->openAtSize(sz);
            label->setColour(c);
            return label;
        }
    }

    return NULL;
}

// grabs and loads all the files specified in the list and returns the number of assets loaded
unsigned int LocalAssetManager::grab(std::initializer_list<std::string> list)
{
    // get the vector size before inserting any new filenames & track the number of loaded assets
    unsigned int start = filenames.size();
    unsigned int loads = NULL;

    // insert the list filenames into the filenames vector
    filenames.insert(filenames.end(), list.begin(), list.end());
    
    // then iterate through the recently inserted filenames
    for (int i = start; i < filenames.size(); i++)
    {
        // get the file type
        std::string str = filenames[i].substr(filenames[i].length() - 4);
        std::string nm = filenames[i].substr(0, filenames[i].length() - 4);
        
        // if it is a image format that is supported
        if (assets->checkTexture(str, nm))
        {
            TextureAsset * texture = new TextureAsset(nm);
            texture->grabFromFile(filenames[i].c_str());
            assets->push(texture);
            ++loads;
            continue;
        } 

        // if its a font file format that is supported
        else if (assets->checkLabel(str, nm)) 
        {
            FontAsset * font = new FontAsset(nm);
            font->grabFromFile(filenames[i].c_str());
            assets->push(font);
            ++loads;
            continue;
        } 
        
        // if its a model format that is supported
        else if (assets->checkModel(str, nm))
        {
            ModelAsset  * model = new ModelAsset(nm);
            model->grabFromFile(filenames[i].c_str());
            assets->push(model);
            ++loads;
            continue;
        }
    }

    // return the number of loaded assets to the user so they know all files specified were loaded
    return(loads);
}
