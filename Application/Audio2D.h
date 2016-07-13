
#pragma once

#include "EngineLayer.h"

class Audio2D {
public:
	virtual ~Audio2D(){}

	virtual void open(std::string filename, bool loop) = 0;
	virtual void pause() = 0;
	virtual void close() = 0;
	virtual void play() = 0;
	virtual void stop() = 0;
};