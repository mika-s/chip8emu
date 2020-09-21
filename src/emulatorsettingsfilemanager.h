#ifndef EMULATORSETTINGSFILEMANAGER_H
#define EMULATORSETTINGSFILEMANAGER_H

#include <QJsonObject>
#include "emulatorsettings.h"


namespace Emu {

class EmulatorSettingsFileManager
{
public:
    explicit EmulatorSettingsFileManager(const QString& filename);
    EmulatorSettings read();
    void write(EmulatorSettings settings);

private:
    QString filename;

    QJsonObject readJson();
    void writeJson(const QJsonObject& json);
};

}

#endif // EMULATORSETTINGSFILEMANAGER_H
