
#include "HighPrecisionTimer.h"

HighPrecisionTimer::HighPrecisionTimer()
{
    m_freq = SDL_GetPerformanceFrequency();
    m_start =- 1;
    m_stop = - 1;
}

HighPrecisionTimer::~HighPrecisionTimer()
{
}

void HighPrecisionTimer::start()
{
    m_start = SDL_GetPerformanceCounter();
}

void HighPrecisionTimer::stop()
{
    m_stop = SDL_GetPerformanceCounter();
}

float HighPrecisionTimer::elapsed(TimeType type)
{
    if (m_start != -1)
    {
        m_stop = SDL_GetPerformanceCounter();
        return(difference(type));
    } 
    
    return -1;
}

void HighPrecisionTimer::clear()
{
    m_start = -1;
    m_stop = -1;
}

float HighPrecisionTimer::difference(TimeType type) const
{
    auto difference = 0.0f;

    switch (type)
    {
    case Seconds:
    {
        difference = static_cast<float>(m_stop - m_start);
        break;
    }

    case Milliseconds:
    {
        difference = static_cast<float>(1.0e3*(m_stop - m_start));
        break;
    }

    case Nanoseconds:
    {
        difference = static_cast<float>(1.0e9*(m_stop - m_start));
        break;
    }
    };

    return(difference / m_freq);
}

unsigned int HighPrecisionTimer::current()
{
    m_current = SDL_GetPerformanceCounter();
    return(m_current);
}

bool HighPrecisionTimer::hasStarted() const
{
    return m_start != -1;
}