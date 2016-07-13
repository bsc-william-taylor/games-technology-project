
#pragma once

#include "Audio2D.h"

class Sound2D : public Audio2D
{
private:
	HCHANNEL soundChannel;
	HSAMPLE soundSample;
	BOOL loop;
	BOOL playing;
public:
	Sound2D();
	~Sound2D();

	void open(std::string filename, bool loop = false);
	void setPlaybackSpeed(double speed);
	void pause();
	void close();
	void play();
	void stop();
	void reset();
};