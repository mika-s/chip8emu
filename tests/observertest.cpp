#include "observertest.h"
#include "emulator.h"
#include "emulatortimer.h"
#include "listenermock.h"


void ObserverTest::initTestCase() {
    timer = std::shared_ptr<Emu::ITimer>(new Emu::EmulatorTimer(this));

    settings = Emu::EmulatorSettings(false, false, false, false, 60, 60, 9, 9,
                                     Emu::Color(0, 0, 0, 0), Emu::Color(0, 0, 0, 0),
                                     Emu::Color(255, 255, 255, 255), Emu::Color(255, 255, 255, 255));
}

void ObserverTest::givenDebugListenersShouldNotifyListenersOnRegisterChanges() {
    Emu::Emulator emulator = Emu::Emulator(timer, settings);
    ListenerMock mock;
    emulator.addDebugListener(mock);
    emulator.loadFile({ 0x71, 0x01 });
    mock.vRegistersChangedWasCalled = false;
    emulator.nextInstruction();

    QVERIFY(mock.vRegistersChangedWasCalled);
}

void ObserverTest::givenErrorListenersShouldNotifyListenersOnLatestErrorChanges() {
    Emu::Emulator emulator = Emu::Emulator(timer, settings);
    ListenerMock mock;
    emulator.addErrorListener(mock);
    emulator.loadFile({ 0x00, 0x00 });
    emulator.nextInstruction();

    QVERIFY(mock.latestErrorChangedWasCalled);
}

void ObserverTest::givenIoListenersShouldNotifyListenersOnDraw() {
    Emu::Emulator emulator = Emu::Emulator(timer, settings);
    ListenerMock mock;
    emulator.addIoListener(mock);
    emulator.loadFile({ 0xD0, 0x00 });
    emulator.nextInstruction();

    QVERIFY(mock.drawWasCalled);
}

void ObserverTest::givenRemovalFromSubscriptionNotNotified() {
    Emu::Emulator emulator = Emu::Emulator(timer, settings);
    ListenerMock mock;

    emulator.addDebugListener(mock);
    emulator.removeDebugListener(mock);
    emulator.loadFile({
        0x71, 0x01,     // V[1] += 01
        0x71, 0x01      // V[1] += 01
    });
    emulator.nextInstruction();

    QVERIFY(!mock.vRegistersChangedWasCalled);

    emulator.addDebugListener(mock);
    emulator.nextInstruction();

    QVERIFY(mock.vRegistersChangedWasCalled);
}

void ObserverTest::cleanupTestCase() {

}
