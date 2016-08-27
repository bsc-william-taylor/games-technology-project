#pragma once

#include "EngineLayer.h"
#include "LocalAssetManager.h"
#include "Media.h"
#include "AudioFile.h"

typedef SDL_Surface Texture;
typedef SDL_Surface Font;

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

    vector<TextureAsset*>& getTextures();
    vector<ModelAsset*>& getModels();
    vector<AudioAsset*>& getAudio();
    vector<FontAsset*>& getFonts();

    bool checkTexture(std::string ext, std::string nm);
    bool checkModel(std::string ext, std::string nm);
    bool checkLabel(std::string ext, std::string nm);

    void recycle();
    void push(TextureAsset*);
    void push(AudioAsset*);
    void push(ModelAsset*);
    void push(FontAsset*);

    LocalAssetManager * grabLocalManager();
};