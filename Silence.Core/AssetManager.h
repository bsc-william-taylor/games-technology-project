#pragma once

#include "EngineLayer.h"
#include "LocalAssetManager.h"
#include "Media.h"
#include "AudioFile.h"

class SILENCE_EXPORT AssetManager
{
    vector<LocalAssetManager*> stateAssetManagers;
    vector<TextureAsset*> textures;
    vector<AudioAsset *> music;
    vector<ModelAsset*> models;
    vector<FontAsset*> fonts;
public:
    AssetManager();
    virtual ~AssetManager();

    void recycle();

    vector<TextureAsset*>& getTextures();
    vector<ModelAsset*>& getModels();
    vector<AudioAsset*>& getAudio();
    vector<FontAsset*>& getFonts();

    bool checkTexture(std::string ext, std::string nm);
    bool checkModel(std::string ext, std::string nm);
    bool checkLabel(std::string ext, std::string nm);

    void push(TextureAsset* asset);
    void push(AudioAsset* asset);
    void push(ModelAsset* asset);
    void push(FontAsset* asset);

    LocalAssetManager * grabLocalManager();
};