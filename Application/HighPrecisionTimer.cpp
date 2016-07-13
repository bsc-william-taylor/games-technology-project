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

#include "HighPrecisionTimer.h"

// Constructor & Deconstructor
HighPrecisionTimer::HighPrecisionTimer()
{
	// Grab frequency of this processor
	m_freq = SDL_GetPerformanceFrequency();
	m_start =- 1;
	m_stop = - 1;
}

HighPrecisionTimer::~HighPrecisionTimer()
{
}

// Records current time in start variable
void HighPrecisionTimer::start()
{
	m_start = SDL_GetPerformanceCounter();
}

// Records current time in stop variable
void HighPrecisionTimer::stop()
{
	m_stop = SDL_GetPerformanceCounter();
}

// Get current time from previous Start call
float HighPrecisionTimer::elapsed(TimeType type)
{
	if (m_start != -1)
	{
		m_stop = SDL_GetPerformanceCounter();
		return(difference(type));
	} 
	else
	{
		return -1;
	}
}

void HighPrecisionTimer::clear()
{
	m_start = -1;
	m_stop = -1;
}

// Time between last Start and Stop calls
float HighPrecisionTimer::difference(TimeType type)
{
	float difference = 0.0f;

	switch (type)
	{
	case SECONDS:
	{
		difference = (float)(m_stop - m_start);
		break;
	}

	case MS:
	{
		difference = (float)(1.0e3*(m_stop - m_start));
		break;
	}

	case NS:
	{
		difference = (float)(1.0e9*(m_stop - m_start));
		break;
	}
	};

	return(difference / m_freq);
}

// Get the current clock count
unsigned int HighPrecisionTimer::current()
{
	m_current = SDL_GetPerformanceCounter();
	return(m_current);
}