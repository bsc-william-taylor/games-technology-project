
#pragma once

#include "Audio2D.h"

class Audio3D : public Audio2D 
{
public:
	virtual ~Audio3D(){}
	virtual void setListenerPosition(float x, float y, float z) = 0;
	virtual void setPosition(float x, float y, float z) = 0;
};