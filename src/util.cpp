#include "util.h"


namespace Util {
    namespace String {

    std::string hexify(std::uint8_t val) {
        std::stringstream ss;
        ss << "0x" << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(val);
        std::string returnVal = ss.str();

        return returnVal;
    }

    std::string hexify(std::uint16_t val) {
        std::stringstream ss;
        ss << "0x" << std::setfill('0') << std::setw(4) << std::hex << val;

        return ss.str();
    }

    QString toQString(bool val) {
        std::stringstream ss;
        ss << val;

        return QString::fromStdString(ss.str());
    }

    QString toQString(int val) {
        std::stringstream ss;
        ss << val;

        return QString::fromStdString(ss.str());
    }

    }

    namespace Color {

    QColor toQColor(Emu::Color color) {
        return QColor(color.r, color.g, color.b, color.a);
    }

    }
}
