#include "util.h"
#include "unrecognizedopcodeexception.h"


namespace Emu {

UnrecognizedOpcodeException::UnrecognizedOpcodeException(std::uint16_t opcode)
    : runtime_error("Unrecognized opcode") {
    makeMessage(opcode);
}

const char* UnrecognizedOpcodeException::what() const noexcept {
    return message.c_str();
}

void UnrecognizedOpcodeException::makeMessage(std::uint16_t opcode) {
    std::stringstream ss;
    ss << runtime_error::what() << ": opcode = " << Util::String::hexify(opcode);

    message = ss.str();
}

}
