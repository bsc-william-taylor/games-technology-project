
#include "LocalAssetManager.h"
#include "AssetManager.h"

LocalAssetManager::LocalAssetManager(AssetManager * assets)
    : assets(assets)
{
}

LocalAssetManager::~LocalAssetManager()
{
}

TextureAsset * LocalAssetManager::getT(std::string filename)
{
    const auto predicate = [&](TextureAsset* texture)
    {
        return texture->getName() == filename;
    };

    const auto textures = assets->getTextures();
    const auto found = std::find_if(textures.begin(), textures.end(), predicate);

    return found == textures.end() ? nullptr : *found;
}

void LocalAssetManager::recycle()
{
    assets->recycle();
}

AudioAsset * LocalAssetManager::getS(std::string filename, AudioCommand load)
{
    const auto audio = new AudioAsset(filename.substr(0, filename.length() - 4));

    switch (load) 
    {
        case StreamLoop: audio->grabFromFile(filename, true, true); break;
        case LoadLoop: audio->grabFromFile(filename, false, true); break;
        case Stream: audio->grabFromFile(filename, true, false); break;
        case Load: audio->grabFromFile(filename, false, false); break;

        default: break;
    };

    assets->push(audio);
    return audio;
}

ModelAsset * LocalAssetManager::getM(std::string filename)
{
    auto models = assets->getModels();

    for (auto& model : models) {
        if (model->getName() == filename) {
            return model;
        }
    }
    
    return nullptr;
}

AudioAsset * LocalAssetManager::getA(std::string filename)
{
    auto audioFiles = assets->getAudio();

    for (auto& audio : audioFiles) {
        if (audio->getName() == filename) {
            return audio;
        }
    }

    return nullptr
;
}

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

    return nullptr;
}

unsigned int LocalAssetManager::grab(std::initializer_list<std::string> list)
{
    auto start = filenames.size();
    auto loads = 0u;

    filenames.insert(filenames.end(), list.begin(), list.end());
    
    for (int i = start; i < filenames.size(); i++)
    {
        auto str = filenames[i].substr(filenames[i].length() - 4);
        auto nm = filenames[i].substr(0, filenames[i].length() - 4);
  
        if (assets->checkTexture(str, nm))
        {
            const auto texture = new TextureAsset(nm);
            texture->grabFromFile(filenames[i].c_str());
            assets->push(texture);
            ++loads;
        }
        else if (assets->checkLabel(str, nm)) 
        {
            const auto font = new FontAsset(nm);
            font->grabFromFile(filenames[i].c_str());
            assets->push(font);
            ++loads;
        } 
        else if (assets->checkModel(str, nm))
        {
            const auto model = new ModelAsset(nm);
            model->grabFromFile(filenames[i].c_str());
            assets->push(model);
            ++loads;
        }
    }

    return(loads);
}
