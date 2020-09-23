#ifndef INSTRUCTIONTEST_H
#define INSTRUCTIONTEST_H

#include <QtTest/QtTest>
#include "emulator.h"


class InstructionTest : public QObject {
    Q_OBJECT

private:
    std::shared_ptr<Emu::ITimer> timer;
    Emu::EmulatorSettings settings;

private slots:
    void initTestCase();
    void given_00E0_clearDisplay();
    void given_1NNN_setPCtoNNN();
    void given_2NNN_callSubroutineAtNNN();
    void given_3XNN_andVXisNNgotoNextPC();
    void given_3XNN_andVXisNotNNdoNotgotoNextPC();
    void given_4XNN_andVXisNotNNgotoNextPC();
    void given_4XNN_andVXisNNdoNotgotoNextPC();
};

#endif // INSTRUCTIONTEST_H
