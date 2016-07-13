
#pragma once

#include "Audio3D.h"

class Sound3D : public Audio3D
{
private:
	HSTREAM musicStream;
public:
	Sound3D();
	~Sound3D();

	void setListenerPosition(float x, float y, float z);
	void setPosition(float x, float y, float z);
	void open(std::string filename, bool loop = false);
	void pause();
	void close();
	void play();
	void stop();
};
