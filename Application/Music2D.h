
#pragma once

#include "Audio2D.h"

class Music2D : public Audio2D
{
private:
	HSTREAM musicStream;
public:
	Music2D();
	~Music2D();

	void open(std::string filename, bool loop = true);
	void pause();
	void close();
	void play();
	void stop();
};