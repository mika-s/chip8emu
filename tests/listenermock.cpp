#include "listenermock.h"

ListenerMock::ListenerMock()
    : opcodeChangedWasCalled(false)
    , vRegistersChangedWasCalled(false)
    , iRegisterChangedWasCalled(false)
    , stackChangedWasCalled(false)
    , pcChangedWasCalled(false)
    , keyChangedWasCalled(false)
    , latestErrorChangedWasCalled(false)
    , delayTimerChangedWasCalled(false)
    , soundTimerChangedWasCalled(false)
    , beepWasCalled(false)
    , drawWasCalled(false)
    , statusChangedWasCalled(false) { }

void ListenerMock::opcodeChanged() {
    opcodeChangedWasCalled = true;
}

void ListenerMock::vRegistersChanged() {
    vRegistersChangedWasCalled = true;
}

void ListenerMock::iRegisterChanged()  {
    iRegisterChangedWasCalled = true;
}

void ListenerMock::stackChanged()  {
    stackChangedWasCalled = true;
}

void ListenerMock::pcChanged()  {
    pcChangedWasCalled = true;
}

void ListenerMock::keyChanged()  {
    keyChangedWasCalled = true;
}

void ListenerMock::latestErrorChanged()  {
    latestErrorChangedWasCalled = true;
}

void ListenerMock::delayTimerChanged()  {
    delayTimerChangedWasCalled = true;
}

void ListenerMock::soundTimerChanged()  {
    soundTimerChangedWasCalled = true;
}

void ListenerMock::beep()  {
    beepWasCalled = true;
}

void ListenerMock::draw()  {
    drawWasCalled = true;
}

void ListenerMock::statusChanged()  {
    statusChangedWasCalled = true;
}
