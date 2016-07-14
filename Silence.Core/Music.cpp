
#include "Music.h"

Music::Music()
{
	source = NULL;
	playing = false;
}

Music::~Music()
{

}

void Music::open(AudioAsset * a)
{
	source = a;
}

void Music::pause()
{
	if (this->hasAudioSource())
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

bool Music::hasAudioSource()
{
	return(source != NULL);
}

void Music::play()
{
	if (this->hasAudioSource() && !playing)
	{
		if (source->isStream())
		{
			BASS_ChannelPlay(source->getStream(), true);
		}
		else
		{
			if (!BASS_ChannelPlay(source->getChannel(), true)) {
				std::cout << BASS_ErrorGetCode() << std::endl;
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
	if (this->hasAudioSource() && playing)
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
