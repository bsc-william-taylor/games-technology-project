
/**
*
* Copyright (c) 2014 : William Taylor : wi11berto@yahoo.co.uk
*
* This software is provided 'as-is', without any
* express or implied warranty. In no event will
* the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute
* it freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented;
*    you must not claim that you wrote the original software.
*    If you use this software in a product, an acknowledgment
*    in the product documentation would be appreciated but
*    is not required.
*
* 2. Altered source versions must be plainly marked as such,
*    and must not be misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source distribution.
*/


#ifndef __HIGH_PRECISION_TIMER_H_
#define __HIGH_PRECISION_TIMER_H_

#include "EngineLayer.h"

// format to return time in
enum TimeType {
	SECONDS,
	MS, NS
};

// Represents a high resolution timer for frame locking
class HighPrecisionTimer
{
private:
	// the current ticks
	unsigned int m_current;
	// the ticks taken during the start() function call
	unsigned int m_start;
	// the ticks taken during the stop() function call
	unsigned int m_stop;
	// the frequency of the processor
	unsigned int m_freq;
public:
	// Constructor & Deconstructor
	HighPrecisionTimer();
	~HighPrecisionTimer();
	
	// get the time difference from start() and stop()
	float difference(TimeType);
	// get the current time eleapsed from start()
	float elapsed(TimeType);
	// start the timer
	void start();
	// stop the timer
	void stop();
	// reset the timers state
	void clear();
	// just returns the current ticks
	unsigned int current();

	bool hasStarted() {
		return m_start != -1;
	}
};

#endif