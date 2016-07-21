
#pragma once

#include "EngineLayer.h"
#include "AudioFile.h"

class SILENCE_EXPORT Music
{
private:
    AudioAsset * source;

    bool playing;
public:
    Music();
    ~Music();

    void open(AudioAsset *);
    void reset();
    void pause();
    void play();
    void stop();

    bool hasAudioSource();
};