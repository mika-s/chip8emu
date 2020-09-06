#ifndef ITIMERLISTENER_H
#define ITIMERLISTENER_H


namespace Emu {

class ITimerListener
{
public:
    virtual ~ITimerListener() = default;
    virtual void tick() = 0;
};

}

#endif // ITIMERLISTENER_H
