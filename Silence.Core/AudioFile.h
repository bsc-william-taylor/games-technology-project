
#pragma once

#include "EngineLayer.h"
#include "TextureAsset.h"
#include "math.h"


class SILENCE_EXPORT AudioAsset 
{
private:
    std::string filename;
    std::string name;

    HSTREAM stream;
    HCHANNEL channel;
    HSAMPLE sample;

    bool _isStream;
public:
    AudioAsset(std::string str);
    ~AudioAsset();

    void grabFromFile(std::string filename, bool stream, bool loop);

    bool isStream();

    HSTREAM getStream();
    HCHANNEL getChannel();

    std::string getName();
};