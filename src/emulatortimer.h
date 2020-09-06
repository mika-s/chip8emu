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
    EmulatorTimer(QObject *parent = nullptr);

    void start(int frequency);
    void stop();
    void notifyOfTick();
    void changeFrequency(int frequency);
    void addTimerListener(ITimerListener& listener);
    void removeTimerListener(ITimerListener& listener);

private:
    std::vector<ITimerListener*> timerListeners;
};

}

#endif // EMULATORTIMER_H
