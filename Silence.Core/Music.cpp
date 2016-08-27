
#include "Music.h"

Music::Music() :
    source(nullptr),
    playing(false)
{
}

Music::~Music()
{
}

void Music::open(AudioAsset * asset)
{
    if(asset)
    {
        source = asset;
    }
    else
    {
        throw Error(Component::Audio, "Asset provided was null", Author::William);
    }
}

void Music::pause()
{
    if (hasAudioSource())
    {
        if (source->isStream())
        {
            BASS_ChannelPause(source->getStream());
        }
        else
        {
            BASS_ChannelPause(source->getChannel());
        }

        playing = false;
    }
}

bool Music::hasAudioSource() const
{
    return source != nullptr;
}

void Music::play()
{
    if (hasAudioSource() && !playing)
    {
        if (source->isStream())
        {
            BASS_ChannelPlay(source->getStream(), true);
        }
        else
        {
            if (!BASS_ChannelPlay(source->getChannel(), true)) 
            {
                std::stringstream ss;
                ss << "Error playing music: ";
                ss << "BASS error code ";
                ss << BASS_ErrorGetCode();
                throw Error(Component::Audio, ss.str(), Author::William);
            }
        }

        playing = true;
    }
}

void Music::reset()
{
    playing = false;
}

void Music::stop()
{
    if (hasAudioSource() && playing)
    {
        if (source->isStream())
        {
            BASS_ChannelStop(source->getStream());
        } 
        else
        {
            BASS_ChannelStop(source->getChannel());
        }

        playing = false;
    }
}
