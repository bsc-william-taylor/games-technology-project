
#include "Audio2D.h"

int Audio2D::GLOBAL_VOLUME = 128;

void Audio2D::toggleMute()
{
	if (GLOBAL_VOLUME == 0.0) {
		GLOBAL_VOLUME = 1.0;
	} else {
		GLOBAL_VOLUME = 0.0;
	}
}