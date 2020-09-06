#ifndef UTIL_H
#define UTIL_H

#include <iomanip>
#include <string>
#include <sstream>
#include <QColor>
#include <QString>
#include "emulatorsettings.h"


namespace Util {
    namespace String {

    std::string hexify(std::uint8_t val);
    std::string hexify(std::uint16_t val);
    QString toQString(bool val);
    QString toQString(int val);

    }

    namespace Color {

    QColor toQColor(Emu::Color color);

    }
}

#endif // UTIL_H
