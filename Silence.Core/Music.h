
#pragma once

#include "EngineLayer.h"
#include "AudioFile.h"

class SILENCE_EXPORT Music
{
    AudioAsset * source;
    bool playing;
public:
    Music();
    virtual ~Music();

    void open(AudioAsset * asset);
    void reset();
    void pause();
    void play();
    void stop();

    bool hasAudioSource() const;
};