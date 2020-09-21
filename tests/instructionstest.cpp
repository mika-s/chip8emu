#include "instructionstest.h"
#include "emulatortimer.h"


void InstructionTest::initTestCase() {
    timer = std::shared_ptr<Emu::ITimer>(new Emu::EmulatorTimer(this));

    settings = Emu::EmulatorSettings(false, false, false, false, 60, 60, 9, 9,
                                     Emu::Color(0, 0, 0, 0), Emu::Color(0, 0, 0, 0),
                                     Emu::Color(255, 255, 255, 255), Emu::Color(255, 255, 255, 255));
}

void InstructionTest::given_00E0_clearDisplay() {
    Emu::Emulator emulator = Emu::Emulator(timer, settings);
    emulator.loadFile({ 0x00, 0xE0 });
    emulator.nextInstruction();

    std::vector<std::vector<bool>> display = emulator.display();

    for (auto & rowIdx : display)
        for (const auto & colIdx : rowIdx)
            QCOMPARE(colIdx, false);
}

void InstructionTest::given_1NNN_setPCtoNNN() {
    Emu::Emulator emulator = Emu::Emulator(timer, settings);
    emulator.loadFile({ 0x12, 0x34 });
    emulator.nextInstruction();

    QCOMPARE(emulator.PC(), 0x234);
}

void InstructionTest::given_2NNN_callSubroutineAtNNN() {
    Emu::Emulator emulator = Emu::Emulator(timer, settings);
    emulator.loadFile({ 0x23, 0x45 });
    emulator.nextInstruction();

    QCOMPARE(emulator.stack().top(), 0x200 + 2); // Old PC (start address + 2) is put on the stack.
                                                 // + 2 to prevent forever loop.
    QCOMPARE(emulator.PC(), 0x345);              // New PC is NNN.
}

void InstructionTest::given_3XNN_andVXisNNgotoNextPC() {
    Emu::Emulator emulator = Emu::Emulator(timer, settings);
    emulator.loadFile({
                          0x62, 0x55,           // V[2] = 0x55
                          0x32, 0x55            // if(V[2] == 0x55) --> gotoNextPC()
                      });

    emulator.nextInstruction();
    QCOMPARE(emulator.PC(), 0x202);             // start address + 2

    emulator.nextInstruction();
    QCOMPARE(emulator.PC(), 0x206);             // PC += 2 ordinary, PC += 2 because if(V[3] == 0x55) is true
}

void InstructionTest::given_3XNN_andVXisNotNNdoNotgotoNextPC() {
    Emu::Emulator emulator = Emu::Emulator(timer, settings);
    emulator.loadFile({
                          0x62, 0x55,           // V[2] = 0x55
                          0x32, 0x56            // if(V[2] == 0x56) --> gotoNextPC()
                      });

    emulator.nextInstruction();
    QCOMPARE(emulator.PC(), 0x202);             // start address + 2

    emulator.nextInstruction();
    QCOMPARE(emulator.PC(), 0x204);             // PC += 2 ordinary, but if test failed, so only ordinary increment
}
