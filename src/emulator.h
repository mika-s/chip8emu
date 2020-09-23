#ifndef EMULATOR_H
#define EMULATOR_H

#include <cstdint>
#include <memory>
#include <random>
#include <stack>
#include <string>
#include <thread>
#include <vector>
#include "idebuglistener.h"
#include "ierrorlistener.h"
#include "iiolistener.h"
#include "itimerlistener.h"
#include "itimer.h"
#include "emulatorstatus.h"
#include "emulatorsettings.h"


namespace Emu {

enum Constants {
      programStartAddress=0x200
    , fontsetStartAddress=0x000
    , displayCols=64
    , displayRows=32
    , fontsetSize=80
    , fontSizePerCharacter=5
    , maxMemory=4096
    , numberOfRegisters=16
    , numberOfKeys=16
    , maxRandValue=255
    , lastBitposInByte=7
};

class Emulator : public ITimerListener
{
public:
    Emulator(const std::shared_ptr<ITimer>& timer, EmulatorSettings settings);
    ~Emulator() override;

    void loadFile(const std::vector<std::uint8_t>& loadedFile) noexcept;
    void start();
    void stop();
    void pause();
    void keyPress(int keyNumber);
    void keyRelease(int keyNumber);
    void tick() override;
    void nextInstruction();

    [[nodiscard]] EmulatorStatus status() const;
    [[nodiscard]] EmulatorSettings settings() const;
    [[nodiscard]] std::uint16_t opcode() const;
    [[nodiscard]] std::vector<std::uint8_t> VRegisters() const;
    [[nodiscard]] std::stack<std::uint16_t> stack() const;
    [[nodiscard]] std::uint16_t IRegister() const;
    [[nodiscard]] std::uint16_t PC() const;
    [[nodiscard]] std::uint8_t delayTimer() const;
    [[nodiscard]] std::uint8_t soundTimer() const;
    [[nodiscard]] std::vector<bool> key() const;
    [[nodiscard]] std::string latestError() const;
    [[nodiscard]] std::vector<std::vector<bool>> display() const;
    void emulatorSettings(EmulatorSettings newSettings);

    void addDebugListener(IDebugListener& listener);
    void removeDebugListener(IDebugListener& listener);
    void addErrorListener(IErrorListener& listener);
    void removeErrorListener(IErrorListener& listener);
    void addIoListener(IIoListener& listener);
    void removeIoListener(IIoListener& listener);

private:
    EmulatorStatus status_;
    EmulatorSettings settings_;
    std::shared_ptr<ITimer> timer_;
    std::mt19937 random;
    std::uniform_int_distribution<int> distribution;
    std::string latestError_;
    std::vector<std::uint8_t> loadedFile_;

    std::vector<IDebugListener*> debugListeners_;
    std::vector<IErrorListener*> errorListeners_;
    std::vector<IIoListener*> ioListeners_;

    /* CPU-specific: */
    std::uint8_t memory_[Emu::Constants::maxMemory];
    bool display_[Emu::Constants::displayCols][Emu::Constants::displayRows];
    std::uint16_t opcode_;
    std::uint16_t PC_;
    std::uint16_t I_;
    std::stack<std::uint16_t> stack_;
    std::uint8_t delayTimer_;
    std::uint8_t soundTimer_;
    std::uint8_t V_[Emu::Constants::numberOfRegisters];
    bool key_[Emu::Constants::numberOfKeys];

    std::uint8_t fontset_[Emu::Constants::fontsetSize] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
    /* CPU-specific end */

    void handleTimers();
    void draw(std::uint8_t X, std::uint8_t Y, std::uint8_t N);
    void init(const std::vector<std::uint8_t>& loadedFile);

    void latestError(std::string latestError);
    void status(EmulatorStatus status);
    void opcode(std::uint16_t newOpcode);
    void PC(std::uint16_t newPC);
    void I(std::uint16_t newI);
    void V(int registerNo, std::uint8_t newValue);
    void delayTimer(std::uint8_t newDelayTimer);
    void soundTimer(std::uint8_t newSoundTimer);
    void soundTimerWithoutBeep(std::uint8_t newSoundTimer);

    void decrementDelayTimer();
    void decrementSoundTimer();

    void resetPC();
    void gotoPreviousPC();
    void gotoNextPC();
    void clearTimers();
    void clearStack();
    void clearMemory();
    void clearDisplay();
    void clearRegisters();
    void clearKeys();
    void loadFontsetIntoMemory();
    void loadFileIntoMemory(const std::vector<std::uint8_t>& loadedFile);

    void notifyDebugListenersAboutOpcode();
    void notifyDebugListenersAboutVregisters();
    void notifyDebugListenersAboutIregister();
    void notifyDebugListenersAboutStack();
    void notifyDebugListenersAboutPC();
    void notifyDebugListenersAboutKey();
    void notifyDebugListenersAboutDelayTimer();
    void notifyDebugListenersAboutSoundTimer();
    void notifyErrorListeners();
    void notifyIoListenersAboutBeep();
    void notifyIoListenersAboutDraw();
    void notifyIoListenersAboutStatus();
};

}

#endif // EMULATOR_H
