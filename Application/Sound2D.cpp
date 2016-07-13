
#include "Sound2D.h"

Sound2D::Sound2D()
	: soundSample(NULL), soundChannel(NULL)
{
	loop = false;
	playing = FALSE;
}

Sound2D::~Sound2D()
{
}

void Sound2D::setPlaybackSpeed(double speed)
{
	if (soundChannel != NULL)
	{
		BASS_ChannelSetAttribute(soundChannel, BASS_ATTRIB_MUSIC_SPEED, (speed * 255));
	}
}

void Sound2D::open(std::string filename, bool loop)
{
	this->loop = loop;

	if (this->loop) {
		soundSample = BASS_SampleLoad(FALSE, filename.c_str(), 0, 0, 3, BASS_SAMPLE_OVER_POS | BASS_SAMPLE_LOOP);
	} else {
		soundSample = BASS_SampleLoad(FALSE, filename.c_str(), 0, 0, 3, BASS_SAMPLE_OVER_POS);
	}
	
	if (soundSample == NULL)
	{
		std::cout << "Error with SAMPLELOAD" << std::endl;
	}

	soundChannel = BASS_SampleGetChannel(soundSample, true);

	if (soundChannel == NULL)
	{
		std::cout << "Error with SAMPLE CHANNED" << std::endl;
	}
}

void Sound2D::pause()
{
	playing = FALSE;

	if (soundChannel != NULL)
	{
		BASS_ChannelPause(soundChannel);
	}
}

void Sound2D::close()
{
	if (soundSample != NULL)
	{
		BASS_SampleFree(soundSample);
	}
}

void Sound2D::play()
{
	if (playing == FALSE)
	{
		if (soundChannel != NULL)
		{
			BASS_ChannelPlay(soundChannel, this->loop);
			playing = TRUE;
		}
	}
}

void Sound2D::stop()
{
	BASS_ChannelStop(soundChannel);
}

void Sound2D::reset()
{
	playing = FALSE;
}