#include <sstream>
#include "pcoutofboundsexception.h"


namespace Emu {

PCOutOfBoundsException::PCOutOfBoundsException(std::uint16_t PC, std::uint16_t max)
    : runtime_error("PC out of bounds") {
    makeMessage(PC, max);
}

const char* PCOutOfBoundsException::what() const noexcept {
    return message.c_str();
}

void PCOutOfBoundsException::makeMessage(std::uint16_t PC, std::uint16_t max) {
    std::stringstream ss;
    ss << runtime_error::what() << ": PC = " << PC << ", max = " << max;

    message = ss.str();
}

}
