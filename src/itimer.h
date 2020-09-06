#ifndef ITIMER_H
#define ITIMER_H

#include "itimerlistener.h"


namespace Emu {

class ITimer
{
public:
    virtual ~ITimer() = default;
    virtual void start(int frequency) = 0;
    virtual void stop() = 0;
    virtual void notifyOfTick() = 0;
    virtual void changeFrequency(int frequency) = 0;
    virtual void addTimerListener(ITimerListener& listener) = 0;
    virtual void removeTimerListener(ITimerListener& listener) = 0;
};

}

#endif // ITIMER_H
