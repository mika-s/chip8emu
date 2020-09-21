#ifndef EMULATORFILELOADER_H
#define EMULATORFILELOADER_H

#include <cstdint>
#include <string>
#include <vector>

namespace Emu {

class EmulatorFileLoader
{
public:
    explicit EmulatorFileLoader(const std::string& filename);
    std::vector<uint8_t> getLoadedFile();

private:
    std::vector<std::uint8_t> loadedFile;

    void readFile(const std::string& filename);
};

}

#endif // EMULATORFILELOADER_H
