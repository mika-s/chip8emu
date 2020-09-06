#include "instructionstest.h"
#include "emulatorsettings.h"
#include "emulatortimer.h"
#include "itimer.h"


bool InstructionTest::myCondition() {
    return true;
}

void InstructionTest::initTestCase() {
    std::shared_ptr<Emu::ITimer> timer = std::shared_ptr<Emu::ITimer>(new Emu::EmulatorTimer(this));
    Emu::EmulatorSettings settings(false, false, false, false, 60, 60, 9, 9,
                                   Emu::Color(0, 0, 0, 0), Emu::Color(0, 0, 0, 0),
                                   Emu::Color(255, 255, 255, 255), Emu::Color(255, 255, 255, 255));

    emulator = std::make_unique<Emu::Emulator>(timer, settings);
}

void InstructionTest::myFirstTest() {
    QVERIFY(true);  // check that a condition is satisfied
    QCOMPARE(1, 1); // compare two values
}

void InstructionTest::mySecondTest() {
    QVERIFY(myCondition());
    QVERIFY(1 != 0);
}

void InstructionTest::cleanupTestCase() {

}
