
#include "LocalAssetManager.h"
#include "AssetManager.h"

LocalAssetManager::LocalAssetManager(AssetManager * assets) : 
    referenceCount(0), 
    assets(assets)
{
}

LocalAssetManager::~LocalAssetManager()
{
}

TextureAsset * LocalAssetManager::newTexture(std::string filename)
{
    const auto predicate = [&](TextureAsset* texture) {
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

AudioAsset * LocalAssetManager::newAudio(std::string filename, AudioCommand load)
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

ModelAsset * LocalAssetManager::newModel(std::string filename)
{
    auto predicate = [&](ModelAsset * m) { return m->getName() == filename; };
    auto models = assets->getModels();
    auto end = models.end();

    auto iterator = std::find_if(models.begin(), end, predicate);

    return iterator == end ? nullptr : *iterator;
}

AudioAsset * LocalAssetManager::newAudio(std::string filename)
{
    auto predicate = [&](AudioAsset * a) { return a->getName() == filename; };
    auto audioFiles = assets->getAudio();
    auto end = audioFiles.end();
    
    auto iterator = std::find_if(audioFiles.begin(), end, predicate);

    return iterator == end ? nullptr : *iterator;
;
}

FontAsset * LocalAssetManager::newFont(std::string font, int sz, SDL_Color c)
{
    auto predicate = [&](FontAsset * f) { return f->getName() == font; };
    auto fonts = assets->getFonts();
    auto end = fonts.end();

    const auto iterator = std::find_if(fonts.begin(), end, predicate);
    
    if(iterator == end)
    {
        return nullptr;
    }
    
    const auto label = *iterator;
    label->openAtSize(sz);
    label->setColour(c);
    return label;
}

int LocalAssetManager::grab(std::initializer_list<std::string> list)
{
    auto loads = 0;
    filenames.insert(filenames.end(), list.begin(), list.end());
    
    for(auto& filename : filenames)
    {
        auto str = filename.substr(filename.length() - 4);
        auto nm = filename.substr(0, filename.length() - 4);

        if (assets->checkTexture(str, nm))
        {
            const auto texture = new TextureAsset(nm);
            texture->grabFromFile(filename.c_str());
            assets->push(texture);
            ++loads;
        }
        else if (assets->checkLabel(str, nm))
        {
            const auto font = new FontAsset(nm);
            font->grabFromFile(filename.c_str());
            assets->push(font);
            ++loads;
        }
        else if (assets->checkModel(str, nm))
        {
            const auto model = new ModelAsset(nm);
            model->grabFromFile(filename.c_str());
            assets->push(model);
            ++loads;
        }
    }

    return loads;
}
