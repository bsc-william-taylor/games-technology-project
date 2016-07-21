
#include "AudioFile.h"

AudioAsset::AudioAsset(std::string str)
{
    this->name = str;
    this->_isStream = false;
}

AudioAsset::~AudioAsset()
{

}

void AudioAsset::grabFromFile(std::string filename, bool stream, bool loop)
{
    this->_isStream = stream;
    this->filename = filename;

    if (_isStream)
    {
        if (loop)
        {
            this->stream = BASS_StreamCreateFile(FALSE, filename.c_str(), NULL, NULL, BASS_SAMPLE_LOOP);
        }
        else
        {
            this->stream = BASS_StreamCreateFile(FALSE, filename.c_str(), NULL, NULL, NULL);
        }
    }
    else
    {
        if (loop)
        {
            this->sample = BASS_SampleLoad(FALSE, filename.c_str(), 0, 0, 3, BASS_SAMPLE_OVER_POS | BASS_SAMPLE_LOOP);
        }
        else
        {
            this->sample = BASS_SampleLoad(FALSE, filename.c_str(), 0, 0, 3, BASS_SAMPLE_OVER_POS);
        }
    
        if (this->sample == NULL)
        {
            std::cout << "Error with SAMPLELOAD" << std::endl;
        }

        this->channel = BASS_SampleGetChannel(sample, true);

        if (this->channel == NULL)
        {
            std::cout << "Error with SAMPLE CHANNEL" << std::endl;
        }
    }
}

bool AudioAsset::isStream()
{
    return this->_isStream;
}

HSTREAM AudioAsset::getStream()
{
    return stream;
}

HCHANNEL AudioAsset::getChannel()
{
    return channel;
}

std::string AudioAsset::getName() 
{
    return this->name;
}