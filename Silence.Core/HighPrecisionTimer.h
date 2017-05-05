
#pragma once

#include "EngineLayer.h"

enum TimeType { Seconds, Milliseconds, Nanoseconds };

class SILENCE_EXPORT HighPrecisionTimer
{
    unsigned int currentTicks;
    unsigned int startTicks;
    unsigned int stopTicks;
    unsigned int freq;
public:
    HighPrecisionTimer();
    virtual ~HighPrecisionTimer();
    
    unsigned int current();

    float difference(TimeType timetype) const;
    float elapsed(TimeType timetype);

    void start();
    void stop();
    void clear();

    bool hasStarted() const;
};