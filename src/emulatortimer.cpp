#include "emulatortimer.h"


namespace Emu {

EmulatorTimer::EmulatorTimer() {
    setTimerType(Qt::PreciseTimer);
    connect(this, &QTimer::timeout, this, &EmulatorTimer::notifyOfTick);
}

EmulatorTimer::EmulatorTimer(QObject *parent)
    : QTimer(parent) {

    setTimerType(Qt::PreciseTimer);
    connect(this, &QTimer::timeout, this, &EmulatorTimer::notifyOfTick);
}

void EmulatorTimer::addTimerListener(ITimerListener& listener) {
    timerListeners_.push_back(&listener);
}

void EmulatorTimer::removeTimerListener(ITimerListener& listener) {
    timerListeners_.erase(std::remove(timerListeners_.begin(), timerListeners_.end(), &listener),
                          timerListeners_.end());
}

void EmulatorTimer::start(int frequency) {
    QTimer::start(std::chrono::milliseconds(static_cast<int>(1000.0 / frequency)));
}

void EmulatorTimer::stop() {
    QTimer::stop();
}

void EmulatorTimer::notifyOfTick() {
    for (ITimerListener* listener : timerListeners_) listener->tick();
}

void EmulatorTimer::changeFrequency(int frequency) {
    QTimer::setInterval(std::chrono::milliseconds(static_cast<int>(1000.0 / frequency)));
}

}
