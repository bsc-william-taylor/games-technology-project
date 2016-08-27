#pragma once

#include "EngineLayer.h"
#include "TextureAsset.h"
#include "FontAsset.h"
#include "ModelFile.h"
#include "AudioFile.h"
#include "Media.h"

class AssetManager;

enum AudioCommand { Stream = 1, Load = 0, StreamLoop = 2, LoadLoop = 3 };
enum LoadCommand { DuplicateFound, InvalidFormat, NotFound };

class SILENCE_EXPORT LocalAssetManager
{
    std::vector<std::string> filenames;
    unsigned int referenceCount;
    AssetManager * assets;
public:
    explicit LocalAssetManager(AssetManager *);
    ~LocalAssetManager();
    
    unsigned int grab(std::initializer_list<std::string>);
   
    TextureAsset * getT(std::string filename);
    AudioAsset * getS(std::string filename, AudioCommand);
    ModelAsset * getM(std::string filename);
    FontAsset * getL(std::string font, int, SDL_Color);
    AudioAsset * getA(std::string filename);

    void recycle();
};