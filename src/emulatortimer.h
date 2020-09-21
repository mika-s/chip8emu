#ifndef EMULATORTIMER_H
#define EMULATORTIMER_H

#include <QTimer>
#include "itimer.h"
#include "itimerlistener.h"

namespace Emu {

class EmulatorTimer : public QTimer, public ITimer
{
public:
    EmulatorTimer();
    explicit EmulatorTimer(QObject *parent = nullptr);

    void start(int frequency) override;
    void stop() override;
    void notifyOfTick() override;
    void changeFrequency(int frequency) override;
    void addTimerListener(ITimerListener& listener) override;
    void removeTimerListener(ITimerListener& listener) override;

private:
    std::vector<ITimerListener*> timerListeners;
};

}

#endif // EMULATORTIMER_H
