#ifndef OBSERVERTEST_H
#define OBSERVERTEST_H

#include <QtTest/QtTest>
#include "emulatorsettings.h"
#include "itimer.h"


class ObserverTest : public QObject {
    Q_OBJECT

private:
    std::shared_ptr<Emu::ITimer> timer;
    Emu::EmulatorSettings settings;

private slots:
    void initTestCase();
    void givenDebugListenersShouldNotifyListenersOnRegisterChanges();
    void givenErrorListenersShouldNotifyListenersOnLatestErrorChanges();
    void givenIoListenersShouldNotifyListenersOnDraw();
    void givenRemovalFromSubscriptionNotNotified();
    void cleanupTestCase();
};

#endif // OBSERVERTEST_H
