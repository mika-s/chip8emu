#ifndef LISTENERMOCK_H
#define LISTENERMOCK_H

#include "idebuglistener.h"
#include "ierrorlistener.h"
#include "iiolistener.h"


class ListenerMock : public Emu::IDebugListener, public Emu::IErrorListener, public Emu::IIoListener {

public:
    ListenerMock();

    void opcodeChanged() override;
    void vRegistersChanged() override;
    void iRegisterChanged() override;
    void stackChanged() override;
    void pcChanged() override;
    void keyChanged() override;
    void latestErrorChanged() override;
    void delayTimerChanged() override;
    void soundTimerChanged() override;
    void beep() override;
    void draw() override;
    void statusChanged() override;

    bool opcodeChangedWasCalled;
    bool vRegistersChangedWasCalled;
    bool iRegisterChangedWasCalled;
    bool stackChangedWasCalled;
    bool pcChangedWasCalled;
    bool keyChangedWasCalled;
    bool latestErrorChangedWasCalled;
    bool delayTimerChangedWasCalled;
    bool soundTimerChangedWasCalled;
    bool beepWasCalled;
    bool drawWasCalled;
    bool statusChangedWasCalled;
};



#endif // LISTENERMOCK_H
