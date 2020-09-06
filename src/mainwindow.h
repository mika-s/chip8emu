#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <unordered_map>
#include <QMainWindow>
#include <QElapsedTimer>
#include "emulator.h"
#include "emulatorsettingsfilemanager.h"
#include "settingsdialog.h"

#define NANO 1e-9


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow, public Emu::IErrorListener, public Emu::IIoListener, public Emu::IDebugListener {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void opcodeChanged() override;
    void vRegistersChanged() override;
    void iRegisterChanged() override;
    void stackChanged() override;
    void pcChanged() override;
    void keyChanged() override;
    void latestErrorChanged() override;
    void delayTimerChanged() override;
    void soundTimerChanged() override;
    void beep() override;
    void draw() override;
    void statusChanged() override;

protected:
    void keyPressEvent(QKeyEvent* keyEvent) override;
    void keyReleaseEvent(QKeyEvent* keyEvent) override;

private:
    void handleDrawFrequencyAverage();
    void handleOpcodeFrequencyAverage();

    Ui::MainWindow *ui;
    std::unique_ptr<Emu::Emulator> emulator;
    std::unique_ptr<Emu::EmulatorSettingsFileManager> settingsFileManager;

    QString filename;
    QString newErrorLine;

    QElapsedTimer drawTimerForAverage;
    QElapsedTimer opcodeTimerForAverage;

    uint updateDrawFrequencyAverageCounter;
    uint updateOpcodeFrequencyAverageCounter;

    uint drawFrequencyAverage;
    uint opcodeFrequencyAverage;

    const uint drawFrequencyAverageFactor = 20;
    const uint opcodeFrequencyAverageFactor = 20;

    const std::unordered_map<int,int> keymap = {
        { Qt::Key::Key_1, 0x1 }
        , { Qt::Key::Key_2, 0x2 }
        , { Qt::Key::Key_3, 0x3 }
        , { Qt::Key::Key_4, 0xC }
        , { Qt::Key::Key_Q, 0x4 }
        , { Qt::Key::Key_W, 0x5 }
        , { Qt::Key::Key_E, 0x6 }
        , { Qt::Key::Key_R, 0xD }
        , { Qt::Key::Key_A, 0x7 }
        , { Qt::Key::Key_S, 0x8 }
        , { Qt::Key::Key_D, 0x9 }
        , { Qt::Key::Key_F, 0xE }
        , { Qt::Key::Key_Z, 0xA }
        , { Qt::Key::Key_X, 0x0 }
        , { Qt::Key::Key_C, 0xB }
        , { Qt::Key::Key_V, 0xF }
    };

    std::unordered_map<int,QPushButton*> keyToButton;

private slots:
    void exit();
    void toggleDebugMenu();
    void startStop();
    void pause();
    void nextInstruction();
    void openFile();
    void openSettingsDialog();
    void buttonPushed(int buttonNumber);
    void buttonReleased(int buttonNumber);
};
#endif // MAINWINDOW_H
