
#include "AssetManager.h"

AssetManager::AssetManager()
{
}

AssetManager::~AssetManager()
{
    for (auto& tex : textures)
    {
        delete tex;
    }

    for (auto& model : models)
    {
        delete model;
    }
        
    for (auto& font : fonts)
    {
        delete font;
    }

    for (auto& audio : music)
    {
        delete audio;
    }

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
    transform(ext.begin(), ext.end(), ext.begin(), tolower);

    if (ext == ".jpg" || ext == ".png" || ext == ".bmp")
    {
        const auto begin = textures.begin();
        const auto end = textures.end();

        return std::find_if(begin, end, [&](TextureAsset * t) { return t->getName() == nm; }) == end;
    }

    return false;
}

bool AssetManager::checkModel(std::string ext, std::string nm)
{
    transform(ext.begin(), ext.end(), ext.begin(), tolower);

    if (ext == ".3ds" || ext == ".obj" || ext == ".md2" || ext == ".md3")
    {
        const auto begin = models.begin();
        const auto end = models.end();

        return std::find_if(begin, end, [&](ModelAsset * m) { return m->getName() == nm; }) == end;
    }

    return false;
}

bool AssetManager::checkLabel(std::string ext, std::string nm)
{
    transform(ext.begin(), ext.end(), ext.begin(), tolower);

    if (ext == ".ttf" || ext == ".otf")
    {
        const auto begin = fonts.begin();
        const auto end = fonts.end();

        return std::find_if(begin, end, [&](FontAsset * f) { return f->getName() == nm; }) == end;
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