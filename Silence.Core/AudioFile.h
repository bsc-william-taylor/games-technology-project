
#pragma once

#include "EngineLayer.h"

class SILENCE_EXPORT AudioAsset 
{
    std::string filename;
    std::string name;

    HCHANNEL channel;
    HSTREAM stream;
    HSAMPLE sample;

    bool isAudioStream;
public:
    explicit AudioAsset(std::string filename);
    virtual ~AudioAsset();

    void grabFromFile(std::string filename, bool stream, bool loop);

    bool isStream() const;

    HCHANNEL getChannel() const;
    HSTREAM getStream() const;
  
    std::string getName() const;
};