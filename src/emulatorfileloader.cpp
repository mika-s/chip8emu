#include <QFile>
#include <vector>
#include "emulatorfileloader.h"


namespace Emu {

EmulatorFileLoader::EmulatorFileLoader(std::string filename) {
    readFile(filename);
}

void EmulatorFileLoader::readFile(std::string filename) {
    QFile file(QString::fromStdString(filename.c_str()));

    if (!file.open(QIODevice::ReadOnly))
        return;

    QByteArray blob = file.readAll();

    for (const char byte : blob)
        loadedFile.push_back(static_cast<uint8_t>(byte));
}

std::vector<uint8_t> EmulatorFileLoader::getLoadedFile() {
    return loadedFile;
}

}
