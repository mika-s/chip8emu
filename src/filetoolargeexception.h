#ifndef FILETOOLARGEEXCEPTION_H
#define FILETOOLARGEEXCEPTION_H

#include <iostream>
#include <exception>
#include <stdexcept>


namespace Emu {

class FileTooLargeException : public std::runtime_error
{
public:
    FileTooLargeException(size_t fileSize, size_t maxSize);
    const char* what() const noexcept;

private:
    void makeMessage(size_t fileSize, size_t maxSize);

    std::string message;
};

}

#endif // FILETOOLARGEEXCEPTION_H
