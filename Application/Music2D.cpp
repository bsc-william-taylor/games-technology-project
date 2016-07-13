
#include "Music2D.h"

Music2D::Music2D()
	: musicStream(NULL)
{
}

Music2D::~Music2D()
{
	close();
}

void Music2D::open(std::string filename, bool loop)
{
	musicStream = BASS_StreamCreateFile(FALSE, filename.c_str(), NULL, NULL, loop ? BASS_SAMPLE_LOOP : NULL);

	if (musicStream == NULL)
	{
		std::cout << "Error MUSIC OPEN" << std::endl;
	}
}

void Music2D::pause()
{
	if (musicStream != NULL)
	{
		BASS_ChannelPause(musicStream);
	}
}

void Music2D::close()
{
	BASS_StreamFree(musicStream);
}

void Music2D::play()
{
	BASS_ChannelPlay(musicStream, TRUE);
}

void Music2D::stop()
{
	 BASS_ChannelStop(musicStream);
}