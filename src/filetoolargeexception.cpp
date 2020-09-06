#include <sstream>
#include "filetoolargeexception.h"


namespace Emu {

FileTooLargeException::FileTooLargeException(size_t fileSize, size_t maxSize)
    : runtime_error("file is too large for memory"){
    makeMessage(fileSize, maxSize);
}

const char* FileTooLargeException::what() const noexcept {
    return message.c_str();
}

void FileTooLargeException::makeMessage(size_t fileSize, size_t maxSize) {
    std::stringstream ss;
    ss << runtime_error::what() << ": file size = " << fileSize << ", max size = " << maxSize;

    message = ss.str();
}

}
