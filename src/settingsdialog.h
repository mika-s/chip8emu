#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "emulatorsettings.h"
#include "ui_settingsdialog.h"


class SettingsDialog : public QDialog, private Ui::SettingsDialog {
    Q_OBJECT

public:
    SettingsDialog(Emu::EmulatorSettings oldSettings, QWidget *parent = nullptr);

    Emu::EmulatorSettings getNewEmulatorSettings() const;

private slots:
    void updateCalculatedFrequency();
    void restoreDefaultValues();
    void changeOffColor();
    void changeOnColor();
    void acceptHandler();

private:
    Emu::EmulatorSettings oldSettings;
    Emu::EmulatorSettings newSettings;

    QColor selectedOffColor;
    QColor selectedOnColor;
};

#endif // SETTINGSDIALOG_H
