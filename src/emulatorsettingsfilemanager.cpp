#include <stdexcept>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include "emulatorsettingsfilemanager.h"


namespace Emu {

static int getIntFromJson(const QJsonObject& json, const QString& field) {
    int result;

    if (json.contains(field) && json[field].isDouble())
        result = json[field].toInt();
    else
        throw std::runtime_error(field.toStdString() + " not found in json file");

    return result;
}

static bool getBoolFromJson(const QJsonObject& json, const QString& field) {
    bool result;

    if (json.contains(field) && json[field].isBool())
        result = json[field].toBool();
    else
        throw std::runtime_error(field.toStdString() + " not found in json file");

    return result;
}

EmulatorSettingsFileManager::EmulatorSettingsFileManager(const QString& filename) {
    this->filename = filename;
}

EmulatorSettings EmulatorSettingsFileManager::read() {
    const QJsonObject root = readJson();

    /* Top-level: */
    bool isIncrementingIinFX55andFX65 = getBoolFromJson(root, "isIncrementingIinFX55andFX65");
    bool isVFaffectedInFX1E = getBoolFromJson(root, "isVFaffectedInFX1E");
    int frequency = getIntFromJson(root, "frequency");
    int instructionsPerFrequencyTick = getIntFromJson(root, "instructionsPerFrequencyTick");

    /* offColor: */
    const QJsonObject offColor = root["offColor"].toObject();

    int offR = getIntFromJson(offColor, "red");
    int offG = getIntFromJson(offColor, "green");
    int offB = getIntFromJson(offColor, "blue");
    int offA = getIntFromJson(offColor, "alpha");

    /* onColor: */
    const QJsonObject onColor = root["onColor"].toObject();

    int onR = getIntFromJson(onColor, "red");
    int onG = getIntFromJson(onColor, "green");
    int onB = getIntFromJson(onColor, "blue");
    int onA = getIntFromJson(onColor, "alpha");

    /* defaultSettings: */
    const QJsonObject defaultSettings = root["defaultSettings"].toObject();

    /* defaultSettings top-level: */
    bool defaultIsIncrementingIinFX55andFX65 = getBoolFromJson(defaultSettings, "isIncrementingIinFX55andFX65");
    bool defaultIsVFaffectedInFX1E = getBoolFromJson(defaultSettings, "isVFaffectedInFX1E");
    int defaultFrequency = getIntFromJson(defaultSettings, "frequency");
    int defaultInstructionsPerFrequencyTick = getIntFromJson(defaultSettings, "instructionsPerFrequencyTick");

    /* defaultSettings offColor: */
    const QJsonObject defaultOffColor = defaultSettings["offColor"].toObject();

    int defaultOffR = getIntFromJson(defaultOffColor, "red");
    int defaultOffG = getIntFromJson(defaultOffColor, "green");
    int defaultOffB = getIntFromJson(defaultOffColor, "blue");
    int defaultOffA = getIntFromJson(defaultOffColor, "alpha");

    /* defaultSettings onColor: */
    const QJsonObject defaultOnColor = defaultSettings["onColor"].toObject();

    int defaultOnR = getIntFromJson(defaultOnColor, "red");
    int defaultOnG = getIntFromJson(defaultOnColor, "green");
    int defaultOnB = getIntFromJson(defaultOnColor, "blue");
    int defaultOnA = getIntFromJson(defaultOnColor, "alpha");


    return EmulatorSettings(  isIncrementingIinFX55andFX65
                            , defaultIsIncrementingIinFX55andFX65
                            , isVFaffectedInFX1E
                            , defaultIsVFaffectedInFX1E
                            , frequency
                            , defaultFrequency
                            , instructionsPerFrequencyTick
                            , defaultInstructionsPerFrequencyTick
                            , Color(offR, offG, offB, offA)
                            , Color(defaultOffR, defaultOffG, defaultOffB, defaultOffA)
                            , Color(onR, onG, onB, onA)
                            , Color(defaultOnR, defaultOnG, defaultOnB, defaultOnA));
}

void EmulatorSettingsFileManager::write(EmulatorSettings settings) {
    QJsonObject root;

    root["isIncrementingIinFX55andFX65"] = settings.isIncrementingIinFX55andFX65;
    root["isVFaffectedInFX1E"] = settings.isVFaffectedInFX1E;
    root["frequency"] = settings.frequency;
    root["instructionsPerFrequencyTick"] = settings.instructionsPerFrequencyTick;

    QJsonObject offColor;
    offColor["red"] = settings.offColor.r;
    offColor["green"] = settings.offColor.g;
    offColor["blue"] = settings.offColor.b;
    offColor["alpha"] = settings.offColor.a;

    root["offColor"] = offColor;

    QJsonObject onColor;
    onColor["red"] = settings.onColor.r;
    onColor["green"] = settings.onColor.g;
    onColor["blue"] = settings.onColor.b;
    onColor["alpha"] = settings.onColor.a;

    root["onColor"] = onColor;

    QJsonObject defaultSettings;
    defaultSettings["isIncrementingIinFX55andFX65"] = settings.defaultIsIncrementingIinFX55andFX65;
    defaultSettings["frequency"] = settings.defaultFrequency;
    defaultSettings["instructionsPerFrequencyTick"] = settings.defaultInstructionsPerFrequencyTick;
    defaultSettings["isVFaffectedInFX1E"] = settings.defaultIsVFaffectedInFX1E;

    QJsonObject defaultOffColor;
    defaultOffColor["red"] = settings.defaultOffColor.r;
    defaultOffColor["green"] = settings.defaultOffColor.g;
    defaultOffColor["blue"] = settings.defaultOffColor.b;
    defaultOffColor["alpha"] = settings.defaultOffColor.a;

    defaultSettings["offColor"] = defaultOffColor;

    QJsonObject defaultOnColor;
    defaultOnColor["red"] = settings.defaultOnColor.r;
    defaultOnColor["green"] = settings.defaultOnColor.g;
    defaultOnColor["blue"] = settings.defaultOnColor.b;
    defaultOnColor["alpha"] = settings.defaultOnColor.a;

    defaultSettings["onColor"] = defaultOnColor;

    root["defaultSettings"] = defaultSettings;

    writeJson(root);
}

QJsonObject EmulatorSettingsFileManager::readJson() {
    QFile settingsFile(filename);

    if (!settingsFile.open(QIODevice::ReadOnly))
        throw std::runtime_error("Cannot find the json file.");

    QByteArray settingsData = settingsFile.readAll();
    settingsFile.close();
    QJsonDocument settingsDoc(QJsonDocument::fromJson(settingsData));
    QJsonObject val = settingsDoc.object();

    return val;
}

void EmulatorSettingsFileManager::writeJson(const QJsonObject& json) {
    QJsonDocument settingsDoc(json);
    QString result(settingsDoc.toJson(QJsonDocument::Indented));

    QFile settingsFile(filename);

    if (!settingsFile.open(QIODevice::WriteOnly | QFile::Truncate))
        throw std::runtime_error("Couldn't open settings file.");

    QTextStream out(&settingsFile);
    out << result;
    settingsFile.close();
}

}
