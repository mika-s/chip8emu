#ifndef UNRECOGNIZEDOPCODEEXCEPTION_H
#define UNRECOGNIZEDOPCODEEXCEPTION_H

#include <iostream>
#include <exception>
#include <stdexcept>


namespace Emu {

class UnrecognizedOpcodeException : public std::runtime_error
{
public:
    UnrecognizedOpcodeException(std::uint16_t opcode);
    const char* what() const noexcept;

private:
    void makeMessage(std::uint16_t opcode);

    std::string message;
};

}

#endif // UNRECOGNIZEDOPCODEEXCEPTION_H
