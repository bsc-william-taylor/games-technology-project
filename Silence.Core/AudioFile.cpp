
#include "AudioFile.h"

AudioAsset::AudioAsset(std::string name) :
    isAudioStream(false),
    sample(NULL),
    stream(NULL),
    name(name)
{
}

AudioAsset::~AudioAsset()
{
}

void AudioAsset::grabFromFile(std::string filename, bool isAudioStream, bool loop)
{
    this->isAudioStream = isAudioStream;
    this->filename = filename;
   
    if (isAudioStream)
    {
        this->stream = BASS_StreamCreateFile(FALSE, filename.c_str(), NULL, NULL, loop ? BASS_SAMPLE_LOOP : NULL);
    }
    else
    {
        const auto args = loop ? BASS_SAMPLE_OVER_POS | BASS_SAMPLE_LOOP : BASS_SAMPLE_OVER_POS;
        sample = BASS_SampleLoad(FALSE, filename.c_str(), 0, 0, 3, args);
    
        if (sample == NULL)
        {
            throw Error(Component::Audio, "Error with SAMPLELOAD", Author::William);
        }

        channel = BASS_SampleGetChannel(sample, true);

        if (channel == NULL)
        {
            throw Error(Component::Audio, "Error with SAMPLE CHANNEL", Author::William);
        }
    }
}

bool AudioAsset::isStream() const
{
    return this->isAudioStream;
}

HSTREAM AudioAsset::getStream() const
{
    return stream;
}

HCHANNEL AudioAsset::getChannel() const
{
    return channel;
}

std::string AudioAsset::getName() const
{
    return this->name;
}