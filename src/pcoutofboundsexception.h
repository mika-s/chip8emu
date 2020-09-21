#ifndef PCOUTOFBOUNDSEXCEPTION_H
#define PCOUTOFBOUNDSEXCEPTION_H

#include <iostream>
#include <exception>
#include <stdexcept>


namespace Emu {

class PCOutOfBoundsException : public std::runtime_error
{
public:
    PCOutOfBoundsException(std::uint16_t PC, std::uint16_t max);
    [[nodiscard]] const char* what() const noexcept override;

private:
    void makeMessage(std::uint16_t PC, std::uint16_t max);

    std::string message;
};

}

#endif // PCOUTOFBOUNDSEXCEPTION_H
