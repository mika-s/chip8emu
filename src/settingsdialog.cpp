#include <QColorDialog>
#include <QMessageBox>
#include <QRegExpValidator>
#include "util.h"
#include "settingsdialog.h"


SettingsDialog::SettingsDialog(Emu::EmulatorSettings oldSettings, QWidget *parent)
    : QDialog(parent)
    , oldSettings(oldSettings)
    , newSettings(Emu::EmulatorSettings())
    , selectedOffColor(QColor(oldSettings.offColor.r
                              , oldSettings.offColor.g
                              , oldSettings.offColor.b
                              , oldSettings.offColor.a))
    , selectedOnColor(QColor(oldSettings.onColor.r
                             , oldSettings.onColor.g
                             , oldSettings.onColor.b
                             , oldSettings.onColor.a)) {
    setupUi(this);

    frequencyInput->setText(Util::String::toQString(oldSettings.frequency));
    instructionsPerFrequencyTickInput->setText(Util::String::toQString(oldSettings.instructionsPerFrequencyTick));
    isIncrementingIinFX55andFX65CheckBox->setChecked(oldSettings.isIncrementingIinFX55andFX65);
    isVFaffectedInFX1ECheckBox->setChecked(oldSettings.isVFaffectedInFX1E);
    updateCalculatedFrequency();

    frequencyInput->setValidator(new QRegExpValidator(QRegExp("\\d*"), this));
    instructionsPerFrequencyTickInput->setValidator(new QRegExpValidator(QRegExp("\\d*"), this));

    connect(okCancelButton, &QDialogButtonBox::accepted, this, &SettingsDialog::acceptHandler);
    connect(okCancelButton, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(frequencyInput, &QLineEdit::textChanged,     this, &SettingsDialog::updateCalculatedFrequency);
    connect(instructionsPerFrequencyTickInput,
            &QLineEdit::textChanged, this, &SettingsDialog::updateCalculatedFrequency);
    connect(restoreDefaultValuesButton, &QPushButton::clicked, this, &SettingsDialog::restoreDefaultValues);
    connect(offColorButton, &QPushButton::clicked, this, &SettingsDialog::changeOffColor);
    connect(onColorButton,  &QPushButton::clicked, this, &SettingsDialog::changeOnColor);
}

Emu::EmulatorSettings SettingsDialog::getNewEmulatorSettings() const {
    return newSettings;
}

void SettingsDialog::updateCalculatedFrequency() {
    int frequency = frequencyInput->text().toInt();
    int instructionsPerFrequencyTick = instructionsPerFrequencyTickInput->text().toInt();

    calculatedFrequencyInput->setText(Util::String::toQString(frequency*instructionsPerFrequencyTick));
}

void SettingsDialog::restoreDefaultValues() {
    frequencyInput->setText(Util::String::toQString(oldSettings.defaultFrequency));
    instructionsPerFrequencyTickInput->setText(
                Util::String::toQString(oldSettings.defaultInstructionsPerFrequencyTick));
    isIncrementingIinFX55andFX65CheckBox->setChecked(oldSettings.defaultIsIncrementingIinFX55andFX65);
    isVFaffectedInFX1ECheckBox->setChecked(oldSettings.defaultIsVFaffectedInFX1E);

    selectedOnColor = Util::Color::toQColor(oldSettings.defaultOnColor);
    selectedOffColor = Util::Color::toQColor(oldSettings.defaultOffColor);
}

void SettingsDialog::changeOffColor() {
    QColorDialog dialog(selectedOffColor, this);

    dialog.exec();
    selectedOffColor = dialog.selectedColor();
}

void SettingsDialog::changeOnColor() {
    QColorDialog dialog(selectedOnColor, this);

    dialog.exec();
    selectedOnColor = dialog.selectedColor();
}

void SettingsDialog::acceptHandler() {
    if (frequencyInput->text().isEmpty() || instructionsPerFrequencyTickInput->text().isEmpty()) {
        (void) QMessageBox::information(this, tr("Missing information"),
                                        tr("Not all of the fields contain information."), QMessageBox::Cancel);
    } else {
        bool isIncrementingIinFX55andFX65 = isIncrementingIinFX55andFX65CheckBox->isChecked();
        bool isVFaffectedInFX1E = isVFaffectedInFX1ECheckBox->isChecked();
        int frequency = frequencyInput->text().toInt();
        int instructionsPerFrequencyTick = instructionsPerFrequencyTickInput->text().toInt();
        int offR, offG, offB, offA;
        selectedOffColor.getRgb(&offR, &offG, &offB, &offA);
        int onR, onG, onB, onA;
        selectedOnColor.getRgb(&onR, &onG, &onB, &onA);

        newSettings = Emu::EmulatorSettings(
                    isIncrementingIinFX55andFX65
                    , oldSettings.defaultIsIncrementingIinFX55andFX65
                    , isVFaffectedInFX1E
                    , oldSettings.defaultIsVFaffectedInFX1E
                    , frequency
                    , oldSettings.defaultFrequency
                    , instructionsPerFrequencyTick
                    , oldSettings.defaultInstructionsPerFrequencyTick
                    , Emu::Color(offR, offG, offB, offA)
                    , Emu::Color(oldSettings.defaultOffColor.r
                                 , oldSettings.defaultOffColor.g
                                 , oldSettings.defaultOffColor.b
                                 , oldSettings.defaultOffColor.a)
                    , Emu::Color(onR, onG, onB, onA)
                    , Emu::Color(oldSettings.defaultOnColor.r
                                 , oldSettings.defaultOnColor.g
                                 , oldSettings.defaultOnColor.b
                                 , oldSettings.defaultOnColor.a));

        accept();
    }
}
