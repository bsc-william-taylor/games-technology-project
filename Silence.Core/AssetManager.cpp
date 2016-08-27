
#include "AssetManager.h"

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
    for (auto& audio : music)
        delete audio;

    stateAssetManagers.clear();
}

void AssetManager::recycle()
{
    for (auto& tex : textures)
    {
        tex->recycle();
    }
}

LocalAssetManager * AssetManager::grabLocalManager()
{
    const auto localManager = new LocalAssetManager(this);
    stateAssetManagers.push_back(localManager);
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
                return false;
            }
        }

        return true;
    }

    return false;
}

vector<TextureAsset*>& AssetManager::getTextures()
{
    return textures;
}

vector<AudioAsset*>& AssetManager::getAudio()
{
    return this->music;
}

vector<ModelAsset*>& AssetManager::getModels()
{
    return models;
}

vector<FontAsset*>& AssetManager::getFonts()
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