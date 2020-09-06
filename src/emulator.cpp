#include <algorithm>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <thread>
#include "emulator.h"
#include "filetoolargeexception.h"
#include "invalidkeyexception.h"
#include "pcoutofboundsexception.h"
#include "unrecognizedopcodeexception.h"


namespace Emu {

Emulator::Emulator(std::shared_ptr<ITimer> timer, EmulatorSettings settings)
    : status_(Stopped)
    , settings_(settings)
    , timer_(timer) {
    timer->addTimerListener(*this);
}

Emulator::~Emulator() {
    timer_->removeTimerListener(*this);
}

void Emulator::addDebugListener(IDebugListener& listener) {
    debugListeners_.push_back(&listener);
}

void Emulator::removeDebugListener(IDebugListener& listener) {
    debugListeners_.erase(std::remove(debugListeners_.begin(), debugListeners_.end(), &listener));
}

void Emulator::addErrorListener(IErrorListener& listener) {
    errorListeners_.push_back(&listener);
}

void Emulator::removeErrorListener(IErrorListener& listener) {
    errorListeners_.erase(std::remove(errorListeners_.begin(), errorListeners_.end(), &listener));
}

void Emulator::addIoListener(IIoListener& listener) {
    ioListeners_.push_back(&listener);
}

void Emulator::removeIoListener(IIoListener& listener) {
    ioListeners_.erase(std::remove(ioListeners_.begin(), ioListeners_.end(), &listener));
}

void Emulator::loadFile(std::vector<std::uint8_t> loadedFile) noexcept {
    stop();
    this->loadedFile_ = loadedFile;

    try {
        init(loadedFile);
    } catch (const FileTooLargeException &e) {
        status(FileNotLoaded);
        latestError(e.what());
        clearMemory();
        clearDisplay();
        resetPC();
        clearStack();
        clearTimers();
        clearRegisters();
        clearKeys();
    }
}

void Emulator::start() {
    if (status_ != Running) {
        status(Running);
        timer_->start(settings_.frequency);
    }
}

void Emulator::stop() {
    bool shouldResetState = status_ != Stopped;
    if (shouldResetState)
        init(loadedFile_);

    status(Stopped);
    timer_->stop();
}

void Emulator::pause() {
    status(Paused);
    timer_->stop();
}

void Emulator::keyPress(int keyNumber) {
    if (keyNumber >= Emu::Constants::numberOfKeys)
        throw InvalidKeyException(keyNumber, Emu::Constants::numberOfKeys);

    key_[keyNumber] = true;
    notifyDebugListenersAboutKey();
}

void Emulator::keyRelease(int keyNumber) {
    if (keyNumber >= Emu::Constants::numberOfKeys)
        throw InvalidKeyException(keyNumber, Emu::Constants::numberOfKeys);

    key_[keyNumber] = false;
    notifyDebugListenersAboutKey();
}

void Emulator::tick() {
    for (int times = 0; times < settings_.instructionsPerFrequencyTick; ++times)
        nextInstruction();

    handleTimers();
}

void Emulator::nextInstruction() {
    try {
        if (PC_ + 1 > Emu::Constants::maxMemory)
            throw PCOutOfBoundsException(PC_, Emu::Constants::maxMemory);

        const std::uint8_t fetched1 = memory_[PC_];
        const std::uint8_t fetched2 = memory_[PC_+1];

        const std::uint16_t fetched = static_cast<uint16_t>(fetched1 << 8 | fetched2);
        opcode(fetched);
        gotoNextPC();

        const std::uint8_t  instruction = (fetched & 0xF000) >> 12;
        const std::uint8_t  X           = (fetched & 0x0F00) >> 8;
        const std::uint8_t  Y           = (fetched & 0x00F0) >> 4;
        const std::uint8_t  N           = (fetched & 0x000F);
        const std::uint8_t  NN          = (fetched & 0x00FF);
        const std::uint16_t NNN         = (fetched & 0x0FFF);

        switch (instruction) {
        case 0x0:
            switch (Y) {
            case 0xE:
                switch (N) {
                case 0x0:   // 00E0     Display disp_clear()
                    clearDisplay();
                    break;
                case 0xE:   // 00EE     Flow    return;
                    PC(stack_.top());
                    stack_.pop();
                    notifyDebugListenersAboutStack();
                    break;
                default:
                    throw UnrecognizedOpcodeException(fetched);
                }
                break;
            default:
                throw UnrecognizedOpcodeException(fetched);
            }
            break;
        case 0x1:           // 1NNN     Flow    goto NNN;
            PC(NNN);
            break;
        case 0x2:           // 2NNN     Flow    *(0xNNN)()
            stack_.push(PC_);
            PC(NNN);
            notifyDebugListenersAboutStack();
            break;
        case 0x3:           // 3XNN     Cond    if(Vx == NN)
            if (V_[X] == NN) gotoNextPC();
            break;
        case 0x4:           // 4XNN     Cond    if(Vx != NN)
            if (V_[X] != NN) gotoNextPC();
            break;
        case 0x5:           // 5XY0     Cond    if(Vx == Vy)
            if (V_[X] == V_[Y]) gotoNextPC();
            break;
        case 0x6:           // 6XNN     Const   Vx = NN
            V(X, NN);
            break;
        case 0x7:           // 7XNN     Const   Vx += NN
            V(X, V_[X] + NN);
            break;
        case 0x8:
            switch (N) {
            case 0x0:       // 8XY0     Assign  Vx = Vy
                V(X, V_[Y]);
                break;
            case 0x1:       // 8XY1     BitOp   Vx = Vx | Vy
                V(X, V_[X] | V_[Y]);
                break;
            case 0x2:       // 8XY2     BitOp   Vx = Vx & Vy
                V(X, V_[X] & V_[Y]);
                break;
            case 0x3:       // 8XY3     BitOp   Vx = Vx ^ Vy
                V(X, V_[X] ^ V_[Y]);
                break;
            case 0x4:       // 8XY4     Math    Vx += Vy
                V(0xF, static_cast<int>(V_[X]) + static_cast<int>(V_[Y]) > UINT8_MAX ? 1 : 0);
                V(X, V_[X] + V_[Y]);
                break;
            case 0x5:       // 8XY5     Math    Vx -= Vy
                V(0xF, V_[X] > V_[Y] ? 1 : 0);
                V(X, V_[X] - V_[Y]);
                break;
            case 0x6:       // 8XY6     BitOp   Vx >>= 1
                V_[0xF] = V_[X] & 1;
                V(X, V_[X] >> 1);
                break;
            case 0x7:       // 8XY7     Math    Vx = Vy - Vx
                V(0xF, V_[Y] > V_[X] ? 1 : 0);
                V(X, V_[Y] - V_[X]);
                break;
            case 0xE:       // 8XYE     BitOp   Vx <<= 1
                V_[0xF] = (V_[X] >> Emu::Constants::lastBitposInByte) & 1;
                V(X, static_cast<uint8_t>(V_[X] << 1));
                break;
            default:
                throw UnrecognizedOpcodeException(fetched);
            }
            break;
        case 0x9:           // 9XY0     Cond    if(Vx != Vy)
            if (V_[X] != V_[Y]) gotoNextPC();
            break;
        case 0xA:           // ANNN     MEM     I = NNN
            I(NNN);
            break;
        case 0xB:           // BNNN     Flow    PC = V0 + NNN
            PC(V_[0] + NNN);
            break;
        case 0xC:           // CXNN     Rand    Vx = rand() & NN
            V(X, (rand() % Emu::Constants::maxRandValue) & NN);
            break;
        case 0xD:           // DXYN     Disp    draw(Vx, Vy, N)
            draw(X, Y, N);
            break;
        case 0xE:
            switch (Y) {
            case 0x9:
                switch (N) {
                case 0xE:   // EX9E     KeyOp   if(key() == Vx)
                    if (key_[V_[X]]) gotoNextPC();
                    break;
                default:
                    throw UnrecognizedOpcodeException(fetched);
                }
                break;
            case 0xA:
                switch (N) {
                case 0x1:   // EXA1     KeyOp   if(key() != Vx)
                    if (!key_[V_[X]]) gotoNextPC();
                    break;
                default:
                    throw UnrecognizedOpcodeException(fetched);
                }
                break;
            default:
                throw UnrecognizedOpcodeException(fetched);
            }
            break;
        case 0xF:
            switch (Y) {
            case 0x0:
                switch (N) {
                case 0x7:   // FX07     Timer   Vx = get_delay()
                    V(X, delayTimer_);
                    break;
                case 0xA: { // FX0A     KeyOp   Vx = get_key()
                    bool isKeyPressed = false;

                    for (int keyIdx = 0; keyIdx < Emu::Constants::numberOfKeys; ++keyIdx) {
                        if (key_[keyIdx]) {
                            isKeyPressed = true;
                            V(X, key_[keyIdx]);
                            break;
                        }
                    }

                    if (!isKeyPressed)
                        gotoPreviousPC();
                    }
                    break;
                default:
                    throw UnrecognizedOpcodeException(fetched);
                }
                break;
            case 0x1:
                switch (N) {
                case 0x5:   // FX15     Timer   delay_timer(Vx)
                    delayTimer(V_[X]);
                    break;
                case 0x8:   // FX18     Sound   sound_timer(Vx)
                    soundTimer(V_[X]);
                    break;
                case 0xE:   // FX1E     MEM     I += Vx
                    if (settings_.isVFaffectedInFX1E)
                        V(0xF, I_ + V_[X] > 0xFFF ? 1 : 0);

                    I(I_ + V_[X]);
                    break;
                default:
                    throw UnrecognizedOpcodeException(fetched);
                }
                break;
            case 0x2:
                switch (N) {
                case 0x9:   // FX29     MEM     I = sprite_addr[Vx]
                    I(Emu::Constants::fontsetStartAddress + Emu::Constants::fontSizePerCharacter * V_[X]);
                    break;
                default:
                    throw UnrecognizedOpcodeException(fetched);
                }
                break;
            case 0x3:
                switch (N) {
                case 0x3:   // FX33     BCD     set_BCD(Vx); (I+0) = BCD(3); (I+1) = BCD(2); (I+2) = BCD(1);
                    memory_[I_]   =  V_[X] / 100;
                    memory_[I_+1] = (V_[X] - memory_[I_] * 100) / 10;
                    memory_[I_+2] =  V_[X] - memory_[I_] * 100 - memory_[I_+1] * 10;
                    break;
                default:
                    throw UnrecognizedOpcodeException(fetched);
                }
                break;
            case 0x5:
                switch (N) {
                case 0x5:   // FX55      MEM     reg_dump(Vx, &I)
                    for (int regIdx = 0; regIdx <= X; ++regIdx)
                        memory_[I_+regIdx] = V_[regIdx];

                    if (settings_.isIncrementingIinFX55andFX65)
                        I(I_ + X + 1);
                    break;
                default:
                    throw UnrecognizedOpcodeException(fetched);
                }
                break;
            case 0x6:
                switch (N) {
                case 0x5:   // FX65     MEM     reg_load(Vx, &I)
                    for (int regIdx = 0; regIdx <= X; ++regIdx)
                        V_[regIdx] = memory_[I_+regIdx];

                    if (settings_.isIncrementingIinFX55andFX65)
                        I(I_ + X + 1);
                    break;
                default:
                    throw UnrecognizedOpcodeException(fetched);
                }
                break;
            default:
                throw UnrecognizedOpcodeException(fetched);
            }
            break;
        default:
            throw UnrecognizedOpcodeException(fetched);
        }
    } catch (const std::runtime_error &e) {
        pause();
        latestError(e.what());
    }
}

void Emulator::handleTimers() {
    if (delayTimer_ > 0)
        decrementDelayTimer();

    if (soundTimer_ > 0)
        decrementSoundTimer();

    notifyIoListenersAboutDraw();
}

/**
 * 0
 * |--------64------->  X
 * |
 * 32
 * |
 * V
 *
 * Y
 */
void Emulator::draw(std::uint8_t X, std::uint8_t Y, std::uint8_t N) {
    const int pixelsWide = 8;
    V(0xF, 0x0);

    for (int rowIdx = 0; rowIdx < N; ++rowIdx) {
        std::uint8_t sprite = memory_[I_+rowIdx];
        std::uint8_t yCoord = (V_[Y] + rowIdx) % Emu::Constants::displayRows;

        for (int spriteIdx = 0; spriteIdx < pixelsWide; ++spriteIdx) {
            std::uint8_t xCoord = (V_[X] + spriteIdx) % Emu::Constants::displayCols;

            bool spritePixel = (sprite >> (pixelsWide - 1 - spriteIdx)) & 1;
            bool screenPixel = display_[xCoord][yCoord];

            if (screenPixel && spritePixel) {
                display_[xCoord][yCoord] = false;
                V(0xF, 0x1);
            } else if (!screenPixel && spritePixel) {
                display_[xCoord][yCoord] = true;
            }

            if (xCoord >= Emu::Constants::displayCols) break;
        }

        if (yCoord >= Emu::Constants::displayRows) break;
    }   
}

void Emulator::init(std::vector<std::uint8_t> loadedFile) {
    clearMemory();
    clearDisplay();
    resetPC();
    clearStack();
    clearTimers();
    clearRegisters();
    clearKeys();
    loadFontsetIntoMemory();
    loadFileIntoMemory(loadedFile);
}

void Emulator::keyPause() {
    timer_->stop();
}

void Emulator::resetPC() {
    PC(Emu::Constants::programStartAddress);
}

void Emulator::gotoPreviousPC() {
    PC(PC_ - 2);
}

void Emulator::gotoNextPC() {
    PC(PC_ + 2);
}

void Emulator::clearTimers() {
    delayTimer(0);
    soundTimerWithoutBeep(0);
}

void Emulator::clearStack() {
    while (!stack_.empty()) stack_.pop();

    notifyDebugListenersAboutStack();
}

void Emulator::clearMemory() {
    memset(memory_, 0, sizeof(memory_[0]) * Emu::Constants::maxMemory);
}

void Emulator::clearDisplay() {
    memset(display_, 0, sizeof(display_[0][0]) * Emu::Constants::displayCols * Emu::Constants::displayRows);
    notifyIoListenersAboutDraw();
}

void Emulator::clearRegisters() {
    I(0);

    for (int rIdx = 0; rIdx < Emu::Constants::numberOfRegisters; ++rIdx)
        V_[rIdx] = 0;

    notifyDebugListenersAboutVregisters();
}

void Emulator::clearKeys() {
    for (int keyIdx = 0; keyIdx < Emu::Constants::numberOfKeys; ++keyIdx)
        key_[keyIdx] = false;

    notifyDebugListenersAboutKey();
}

void Emulator::loadFontsetIntoMemory() {
    for(int mIdx = 0; mIdx < Emu::Constants::fontsetSize; ++mIdx)
        memory_[Emu::Constants::fontsetStartAddress + mIdx] = fontset_[mIdx];
}

void Emulator::loadFileIntoMemory(std::vector<std::uint8_t> loadedFile) {
    if (loadedFile.size() > Emu::Constants::programStartAddress + Emu::Constants::maxMemory)
        throw FileTooLargeException(loadedFile.size(), Emu::Constants::programStartAddress + Emu::Constants::maxMemory);

    unsigned int address = Emu::Constants::programStartAddress;
    for (size_t ldIdx = 0; ldIdx < loadedFile.size(); ++ldIdx) {
        memory_[address] = loadedFile[ldIdx];
        ++address;
    }
}

void Emulator::decrementDelayTimer() {
    delayTimer(delayTimer_ - 1);
}

void Emulator::decrementSoundTimer() {
    soundTimer(soundTimer_ - 1);
}

EmulatorStatus Emulator::status() const {
    return status_;
}

EmulatorSettings Emulator::settings() const {
    return settings_;
}

std::uint8_t Emulator::displayRows() const {
    return Emu::Constants::displayRows;
}

std::uint8_t Emulator::displayCols() const {
    return Emu::Constants::displayCols;
}

std::uint16_t Emulator::opcode() const {
    return opcode_;
}

std::vector<std::uint8_t> Emulator::VRegisters() const {
    std::vector<std::uint8_t> vAsVector;

    for (size_t regIdx = 0; regIdx < Emu::Constants::numberOfRegisters; ++regIdx)
        vAsVector.push_back(V_[regIdx]);

    return vAsVector;
}

std::stack<std::uint16_t> Emulator::stack() const {
    return stack_;
}

std::uint16_t Emulator::IRegister() const {
    return I_;
}

std::uint16_t Emulator::PC() const {
    return PC_;
}

std::uint8_t Emulator::delayTimer() const {
    return delayTimer_;
}

std::uint8_t Emulator::soundTimer() const {
    return soundTimer_;
}

std::vector<bool> Emulator::key() const {
    std::vector<bool> keysAsVector;

    for (size_t keyIdx = 0; keyIdx < Emu::Constants::numberOfKeys; ++keyIdx)
        keysAsVector.push_back(key_[keyIdx]);

    return keysAsVector;
}

std::string Emulator::latestError() const {
    return latestError_;
}

std::vector<std::vector<bool>> Emulator::display() const {
    std::vector<std::vector<bool>> displayAsVector;

    for (int rowIdx = 0; rowIdx < Emu::Constants::displayRows; ++rowIdx) {
        std::vector<bool> row;

        for (int colIdx = 0; colIdx < Emu::Constants::displayCols; ++colIdx)
            row.push_back(display_[colIdx][rowIdx]);

        displayAsVector.push_back(row);
    }

    return displayAsVector;
}

void Emulator::emulatorSettings(EmulatorSettings newSettings) {
    this->settings_ = newSettings;
    timer_->changeFrequency(newSettings.frequency);
}

void Emulator::latestError(std::string latestError) {
    this->latestError_ = latestError;
    notifyErrorListeners();
}

void Emulator::status(EmulatorStatus status) {
    this->status_ = status;
    notifyIoListenersAboutStatus();
}

void Emulator::opcode(std::uint16_t newOpcode) {
    opcode_ = newOpcode;
    notifyDebugListenersAboutOpcode();
}

void Emulator::PC(std::uint16_t newPC) {
    PC_ = newPC;
    notifyDebugListenersAboutPC();
}

void Emulator::I(std::uint16_t newI) {
    I_ = newI;
    notifyDebugListenersAboutIregister();
}

void Emulator::V(int registerNo, std::uint8_t newValue) {
    V_[registerNo] = newValue;
    notifyDebugListenersAboutVregisters();
}

void Emulator::delayTimer(std::uint8_t newDelayTimer) {
    delayTimer_ = newDelayTimer;
    notifyDebugListenersAboutDelayTimer();
}

void Emulator::soundTimer(std::uint8_t newSoundTimer) {
    soundTimer_ = newSoundTimer;
    notifyDebugListenersAboutSoundTimer();

    if (soundTimer_ == 0)
        notifyIoListenersAboutBeep();
}

void Emulator::soundTimerWithoutBeep(std::uint8_t newSoundTimer) {
    soundTimer_ = newSoundTimer;
    notifyDebugListenersAboutSoundTimer();
}

void Emulator::notifyDebugListenersAboutOpcode() {
    for (IDebugListener* listener : debugListeners_) listener->opcodeChanged();
}

void Emulator::notifyDebugListenersAboutVregisters() {
    for (IDebugListener* listener : debugListeners_) listener->vRegistersChanged();
}

void Emulator::notifyDebugListenersAboutIregister() {
    for (IDebugListener* listener : debugListeners_) listener->iRegisterChanged();
}

void Emulator::notifyDebugListenersAboutStack() {
    for (IDebugListener* listener : debugListeners_) listener->stackChanged();
}

void Emulator::notifyDebugListenersAboutPC() {
    for (IDebugListener* listener : debugListeners_) listener->pcChanged();
}

void Emulator::notifyDebugListenersAboutKey() {
    for (IDebugListener* listener : debugListeners_) listener->keyChanged();
}

void Emulator::notifyDebugListenersAboutDelayTimer() {
    for (IDebugListener* listener : debugListeners_) listener->delayTimerChanged();
}

void Emulator::notifyDebugListenersAboutSoundTimer() {
    for (IDebugListener* listener : debugListeners_) listener->soundTimerChanged();
}

void Emulator::notifyErrorListeners() {
    for (IErrorListener* listener : errorListeners_) listener->latestErrorChanged();
}

void Emulator::notifyIoListenersAboutBeep() {
    for (IIoListener* listener : ioListeners_) listener->beep();
}

void Emulator::notifyIoListenersAboutDraw() {
    for (IIoListener* listener : ioListeners_) listener->draw();
}

void Emulator::notifyIoListenersAboutStatus() {
    for (IIoListener* listener : ioListeners_) listener->statusChanged();
}

}
