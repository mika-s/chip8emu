#include <sstream>
#include "invalidkeyexception.h"


namespace Emu {

InvalidKeyException::InvalidKeyException(int keyNumber, int max)
    : runtime_error("Invalid key") {
    makeMessage(keyNumber, max);
}

const char* InvalidKeyException::what() const noexcept {
    return message.c_str();
}

void InvalidKeyException::makeMessage(int keyNumber, int max) {
    std::stringstream ss;
    ss << runtime_error::what() << ": keyNumber = " << keyNumber << ", max = " << max;

    message = ss.str();
}

}
