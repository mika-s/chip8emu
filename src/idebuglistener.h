#ifndef IDEBUGLISTENER_H
#define IDEBUGLISTENER_H


namespace Emu {

class IDebugListener {

public:
    virtual ~IDebugListener() = default;
    virtual void opcodeChanged() = 0;
    virtual void vRegistersChanged() = 0;
    virtual void iRegisterChanged() = 0;
    virtual void stackChanged() = 0;
    virtual void pcChanged() = 0;
    virtual void keyChanged() = 0;
    virtual void delayTimerChanged() = 0;
    virtual void soundTimerChanged() = 0;
};

}

#endif // IDEBUGLISTENER_H
