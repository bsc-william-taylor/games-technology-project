#pragma once

#include "EngineLayer.h"
#include "TextureAsset.h"
#include "FontAsset.h"
#include "ModelFile.h"
#include "AudioFile.h"
#include "Media.h"

class AssetManager;

enum AudioCommand { Load, Stream, StreamLoop, LoadLoop };
enum LoadCommand { DuplicateFound, InvalidFormat, NotFound };

class SILENCE_EXPORT LocalAssetManager
{
    std::vector<std::string> filenames;
    int referenceCount;
    AssetManager * assets;
public:
    explicit LocalAssetManager(AssetManager *);
    ~LocalAssetManager();
    
    int grab(std::initializer_list<std::string>);
   
    AudioAsset * newAudio(std::string filename, AudioCommand playbackStyle);
    AudioAsset * newAudio(std::string filename);

    TextureAsset * newTexture(std::string filename);
    ModelAsset * newModel(std::string filename);
    FontAsset * newFont(std::string font, int pt, SDL_Color colour);
    
    void recycle();
};