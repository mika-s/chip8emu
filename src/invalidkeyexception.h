#ifndef INVALIDKEYEXCEPTION_H
#define INVALIDKEYEXCEPTION_H

#include <iostream>
#include <exception>
#include <stdexcept>


namespace Emu {

class InvalidKeyException : public std::runtime_error
{
public:
    InvalidKeyException(int keyNumber, int max);
    [[nodiscard]] const char* what() const noexcept override;

private:
    void makeMessage(int keyNumber, int max);

    std::string message;
};

}

#endif // INVALIDKEYEXCEPTION_H
