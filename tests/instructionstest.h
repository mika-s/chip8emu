#ifndef INSTRUCTIONTEST_H
#define INSTRUCTIONTEST_H

#include <QtTest/QtTest>
#include "emulator.h"


class InstructionTest : public QObject {
    Q_OBJECT

private:
    bool myCondition();

    std::unique_ptr<Emu::Emulator> emulator;

private slots:
    void initTestCase();
    void myFirstTest();
    void mySecondTest();
    void cleanupTestCase();
};

#endif // INSTRUCTIONTEST_H
