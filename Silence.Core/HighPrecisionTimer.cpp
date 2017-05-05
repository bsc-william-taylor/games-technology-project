
#include "HighPrecisionTimer.h"

HighPrecisionTimer::HighPrecisionTimer()
{
    freq = SDL_GetPerformanceFrequency();
    startTicks = -1;
    stopTicks = -1;
}

HighPrecisionTimer::~HighPrecisionTimer()
{
}

void HighPrecisionTimer::start()
{
    startTicks = SDL_GetPerformanceCounter();
}

void HighPrecisionTimer::stop()
{
    stopTicks = SDL_GetPerformanceCounter();
}

float HighPrecisionTimer::elapsed(TimeType type)
{
    if (startTicks != -1)
    {
        stopTicks = SDL_GetPerformanceCounter();
        return(difference(type));
    }

    return -1;
}

void HighPrecisionTimer::clear()
{
    startTicks = -1;
    stopTicks = -1;
}

float HighPrecisionTimer::difference(TimeType type) const
{
    auto difference = 0.0f;

    switch (type)
    {
        case Seconds:
        {
            difference = static_cast<float>(stopTicks - startTicks);
            break;
        }

        case Milliseconds:
        {
            difference = static_cast<float>(1.0e3*(stopTicks - startTicks));
            break;
        }

        case Nanoseconds:
        {
            difference = static_cast<float>(1.0e9*(stopTicks - startTicks));
            break;
        }
    };

    return(difference / freq);
}

unsigned int HighPrecisionTimer::current()
{
    currentTicks = SDL_GetPerformanceCounter();
    return(currentTicks);
}

bool HighPrecisionTimer::hasStarted() const
{
    return startTicks != -1;
}