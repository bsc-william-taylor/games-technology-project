
#pragma once

#include "Audio3D.h"

class Music3D : public Audio3D
{
private:
	HSTREAM musicStream;
public:
	Music3D();
	~Music3D();

	void setListenerPosition(float x, float y, float z);
	void setPosition(float x, float y, float z);
	void open(std::string filename);
	void pause();
	void close();
	void play();
	void stop();
};
