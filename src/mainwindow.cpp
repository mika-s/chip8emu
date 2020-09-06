#include <memory>
#include <sstream>
#include <iostream>
#include <QFileDialog>
#include <QMessageBox>
#include <QSound>
#include <QTimer>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "emulator.h"
#include "emulatorfileloader.h"
#include "emulatortimer.h"
#include "emulatorsettingsfilemanager.h"
#include "emulatorsettings.h"
#include "settingsdialog.h"
#include "util.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , drawTimerForAverage(QElapsedTimer())
    , opcodeTimerForAverage(QElapsedTimer())
    , updateDrawFrequencyAverageCounter(0)
    , updateOpcodeFrequencyAverageCounter(0) {

    ui->setupUi(this);

    settingsFileManager = std::make_unique<Emu::EmulatorSettingsFileManager>(
                QCoreApplication::applicationDirPath() + "/settings.json");

    try {
        Emu::EmulatorSettings settings = settingsFileManager->read();

        std::shared_ptr<Emu::ITimer> timer = std::shared_ptr<Emu::ITimer>(new Emu::EmulatorTimer(this));
        emulator = std::make_unique<Emu::Emulator>(timer, settings);

        emulator->addDebugListener(*this);
        emulator->addErrorListener(*this);
        emulator->addIoListener(*this);

        ui->gameRenderArea->offColor(Util::Color::toQColor(settings.offColor));
        ui->gameRenderArea->onColor(Util::Color::toQColor(settings.onColor));
    }  catch (const std::runtime_error &e) {
        QMessageBox messageBox;
        messageBox.critical(this, "Error", e.what());
        messageBox.show();

        QTimer* exitTimer = new QTimer(this);
        connect(exitTimer, &QTimer::timeout, this, &MainWindow::exit);
        exitTimer->setSingleShot(true);
        exitTimer->start(0);
    }

    ui->debugwrapperWidget->hide();
    ui->startStopButton->setEnabled(false);
    ui->pauseButton->setEnabled(false);
    ui->nextInstrButton->setEnabled(false);

    keyToButton.insert({ Qt::Key::Key_1, ui->oneButton });
    keyToButton.insert({ Qt::Key::Key_2, ui->twoButton });
    keyToButton.insert({ Qt::Key::Key_3, ui->threeButton });
    keyToButton.insert({ Qt::Key::Key_4, ui->cButton });
    keyToButton.insert({ Qt::Key::Key_Q, ui->fourButton });
    keyToButton.insert({ Qt::Key::Key_W, ui->fiveButton });
    keyToButton.insert({ Qt::Key::Key_E, ui->sixButton });
    keyToButton.insert({ Qt::Key::Key_R, ui->dButton });
    keyToButton.insert({ Qt::Key::Key_A, ui->sevenButton });
    keyToButton.insert({ Qt::Key::Key_S, ui->eightButton });
    keyToButton.insert({ Qt::Key::Key_D, ui->nineButton });
    keyToButton.insert({ Qt::Key::Key_F, ui->eButton });
    keyToButton.insert({ Qt::Key::Key_Z, ui->aButton });
    keyToButton.insert({ Qt::Key::Key_X, ui->zeroButton });
    keyToButton.insert({ Qt::Key::Key_C, ui->bButton });
    keyToButton.insert({ Qt::Key::Key_V, ui->fButton });

    QKeyEvent oneKeyPress(QEvent::Type::KeyPress, Qt::Key::Key_1, Qt::KeyboardModifier::NoModifier);
    QKeyEvent oneKeyRelease(QEvent::Type::KeyRelease, Qt::Key::Key_1, Qt::KeyboardModifier::NoModifier);
    QKeyEvent twoKeyPress(QEvent::Type::KeyPress, Qt::Key::Key_2, Qt::KeyboardModifier::NoModifier);
    QKeyEvent twoKeyRelease(QEvent::Type::KeyRelease, Qt::Key::Key_2, Qt::KeyboardModifier::NoModifier);
    QKeyEvent threeKeyPress(QEvent::Type::KeyPress, Qt::Key::Key_3, Qt::KeyboardModifier::NoModifier);
    QKeyEvent threeKeyRelease(QEvent::Type::KeyRelease, Qt::Key::Key_3, Qt::KeyboardModifier::NoModifier);
    QKeyEvent fourKeyPress(QEvent::Type::KeyPress, Qt::Key::Key_4, Qt::KeyboardModifier::NoModifier);
    QKeyEvent fourKeyRelease(QEvent::Type::KeyRelease, Qt::Key::Key_4, Qt::KeyboardModifier::NoModifier);
    QKeyEvent qKeyPress(QEvent::Type::KeyPress, Qt::Key::Key_Q, Qt::KeyboardModifier::NoModifier);
    QKeyEvent qKeyRelease(QEvent::Type::KeyRelease, Qt::Key::Key_Q, Qt::KeyboardModifier::NoModifier);
    QKeyEvent wKeyPress(QEvent::Type::KeyPress, Qt::Key::Key_W, Qt::KeyboardModifier::NoModifier);
    QKeyEvent wKeyRelease(QEvent::Type::KeyRelease, Qt::Key::Key_W, Qt::KeyboardModifier::NoModifier);
    QKeyEvent eKeyPress(QEvent::Type::KeyPress, Qt::Key::Key_E, Qt::KeyboardModifier::NoModifier);
    QKeyEvent eKeyRelease(QEvent::Type::KeyRelease, Qt::Key::Key_E, Qt::KeyboardModifier::NoModifier);
    QKeyEvent rKeyPress(QEvent::Type::KeyPress, Qt::Key::Key_R, Qt::KeyboardModifier::NoModifier);
    QKeyEvent rKeyRelease(QEvent::Type::KeyRelease, Qt::Key::Key_R, Qt::KeyboardModifier::NoModifier);
    QKeyEvent aKeyPress(QEvent::Type::KeyPress, Qt::Key::Key_A, Qt::KeyboardModifier::NoModifier);
    QKeyEvent aKeyRelease(QEvent::Type::KeyRelease, Qt::Key::Key_A, Qt::KeyboardModifier::NoModifier);
    QKeyEvent sKeyPress(QEvent::Type::KeyPress, Qt::Key::Key_S, Qt::KeyboardModifier::NoModifier);
    QKeyEvent sKeyRelease(QEvent::Type::KeyRelease, Qt::Key::Key_S, Qt::KeyboardModifier::NoModifier);
    QKeyEvent dKeyPress(QEvent::Type::KeyPress, Qt::Key::Key_D, Qt::KeyboardModifier::NoModifier);
    QKeyEvent dKeyRelease(QEvent::Type::KeyRelease, Qt::Key::Key_D, Qt::KeyboardModifier::NoModifier);
    QKeyEvent fKeyPress(QEvent::Type::KeyPress, Qt::Key::Key_F, Qt::KeyboardModifier::NoModifier);
    QKeyEvent fKeyRelease(QEvent::Type::KeyRelease, Qt::Key::Key_F, Qt::KeyboardModifier::NoModifier);
    QKeyEvent zKeyPress(QEvent::Type::KeyPress, Qt::Key::Key_Z, Qt::KeyboardModifier::NoModifier);
    QKeyEvent zKeyRelease(QEvent::Type::KeyRelease, Qt::Key::Key_Z, Qt::KeyboardModifier::NoModifier);
    QKeyEvent xKeyPress(QEvent::Type::KeyPress, Qt::Key::Key_X, Qt::KeyboardModifier::NoModifier);
    QKeyEvent xKeyRelease(QEvent::Type::KeyRelease, Qt::Key::Key_X, Qt::KeyboardModifier::NoModifier);
    QKeyEvent cKeyPress(QEvent::Type::KeyPress, Qt::Key::Key_C, Qt::KeyboardModifier::NoModifier);
    QKeyEvent cKeyRelease(QEvent::Type::KeyRelease, Qt::Key::Key_C, Qt::KeyboardModifier::NoModifier);
    QKeyEvent vKeyPress(QEvent::Type::KeyPress, Qt::Key::Key_V, Qt::KeyboardModifier::NoModifier);
    QKeyEvent vKeyRelease(QEvent::Type::KeyRelease, Qt::Key::Key_V, Qt::KeyboardModifier::NoModifier);

    connect(ui->openCloseDebugButton,   &QPushButton::clicked, this, &MainWindow::toggleDebugMenu);
    connect(ui->startStopButton,        &QPushButton::clicked, this, &MainWindow::startStop);
    connect(ui->pauseButton,            &QPushButton::clicked, this, &MainWindow::pause);
    connect(ui->nextInstrButton,        &QPushButton::clicked, this, &MainWindow::nextInstruction);
    connect(ui->actionOpenFile,         &QAction::triggered,   this, &MainWindow::openFile);
    connect(ui->actionOrdinarySettings, &QAction::triggered,   this, &MainWindow::openSettingsDialog);
    connect(ui->zeroButton,             &QPushButton::pressed, this, [this](){ buttonPushed(0x0); });
    connect(ui->zeroButton,             &QPushButton::released, this, [this](){ buttonReleased(0x0); });
    connect(ui->oneButton,              &QPushButton::pressed, this, [this](){ buttonPushed(0x1); });
    connect(ui->oneButton,              &QPushButton::released, this, [this](){ buttonReleased(0x1); });
    connect(ui->twoButton,              &QPushButton::pressed, this, [this](){ buttonPushed(0x2); });
    connect(ui->twoButton,              &QPushButton::released, this, [this](){ buttonReleased(0x2); });
    connect(ui->threeButton,            &QPushButton::pressed, this, [this](){ buttonPushed(0x3); });
    connect(ui->threeButton,            &QPushButton::released, this, [this](){ buttonReleased(0x3); });
    connect(ui->fourButton,             &QPushButton::pressed, this, [this](){ buttonPushed(0x4); });
    connect(ui->fourButton,             &QPushButton::released, this, [this](){ buttonReleased(0x4); });
    connect(ui->fiveButton,             &QPushButton::pressed, this, [this](){ buttonPushed(0x5); });
    connect(ui->fiveButton,             &QPushButton::released, this, [this](){ buttonReleased(0x5); });
    connect(ui->sixButton,              &QPushButton::pressed, this, [this](){ buttonPushed(0x6); });
    connect(ui->sixButton,              &QPushButton::released, this, [this](){ buttonReleased(0x6); });
    connect(ui->sevenButton,            &QPushButton::pressed, this, [this](){ buttonPushed(0x7); });
    connect(ui->sevenButton,            &QPushButton::released, this, [this](){ buttonReleased(0x7); });
    connect(ui->eightButton,            &QPushButton::pressed, this, [this](){ buttonPushed(0x8); });
    connect(ui->eightButton,            &QPushButton::released, this, [this](){ buttonReleased(0x8); });
    connect(ui->nineButton,             &QPushButton::pressed, this, [this](){ buttonPushed(0x9); });
    connect(ui->nineButton,             &QPushButton::released, this, [this](){ buttonReleased(0x9); });
    connect(ui->aButton,                &QPushButton::pressed, this, [this](){ buttonPushed(0xA); });
    connect(ui->aButton,                &QPushButton::released, this, [this](){ buttonReleased(0xA); });
    connect(ui->bButton,                &QPushButton::pressed, this, [this](){ buttonPushed(0xB); });
    connect(ui->bButton,                &QPushButton::released, this, [this](){ buttonReleased(0xB); });
    connect(ui->cButton,                &QPushButton::pressed, this, [this](){ buttonPushed(0xC); });
    connect(ui->cButton,                &QPushButton::released, this, [this](){ buttonReleased(0xC); });
    connect(ui->dButton,                &QPushButton::pressed, this, [this](){ buttonPushed(0xD); });
    connect(ui->dButton,                &QPushButton::released, this, [this](){ buttonReleased(0xD); });
    connect(ui->eButton,                &QPushButton::pressed, this, [this](){ buttonPushed(0xE); });
    connect(ui->eButton,                &QPushButton::released, this, [this](){ buttonReleased(0xE); });
    connect(ui->fButton,                &QPushButton::pressed, this, [this](){ buttonPushed(0xF); });
    connect(ui->fButton,                &QPushButton::released, this, [this](){ buttonReleased(0xF); });
}

MainWindow::~MainWindow() {
    if (emulator != nullptr) {
        emulator->removeDebugListener(*this);
        emulator->removeErrorListener(*this);
        emulator->removeIoListener(*this);
    }

    delete ui;
}

void MainWindow::exit() {
    QApplication::exit(1);
}

void MainWindow::keyPressEvent(QKeyEvent* keyEvent) {
    int key = keyEvent->key();
    if (keymap.find(key) != keymap.end()) {
        emulator->keyPress(keymap.at(key));
        keyToButton.at(key)->setDown(true);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent* keyEvent) {
    int key = keyEvent->key();
    if (keymap.find(key) != keymap.end()) {
        emulator->keyRelease(keymap.at(key));
        keyToButton.at(key)->setDown(false);
    }
}

void MainWindow::buttonPushed(int buttonNumber) {
    emulator->keyPress(buttonNumber);
}

void MainWindow::buttonReleased(int buttonNumber) {
    emulator->keyRelease(buttonNumber);
}

void MainWindow::vRegistersChanged() {
    std::vector<std::uint8_t> V = emulator->VRegisters();
    ui->V0LineEdit->setText(QString::fromStdString(Util::String::hexify(V[0x0])));
    ui->V1LineEdit->setText(QString::fromStdString(Util::String::hexify(V[0x1])));
    ui->V2LineEdit->setText(QString::fromStdString(Util::String::hexify(V[0x2])));
    ui->V3LineEdit->setText(QString::fromStdString(Util::String::hexify(V[0x3])));
    ui->V4LineEdit->setText(QString::fromStdString(Util::String::hexify(V[0x4])));
    ui->V5LineEdit->setText(QString::fromStdString(Util::String::hexify(V[0x5])));
    ui->V6LineEdit->setText(QString::fromStdString(Util::String::hexify(V[0x6])));
    ui->V7LineEdit->setText(QString::fromStdString(Util::String::hexify(V[0x7])));
    ui->V8LineEdit->setText(QString::fromStdString(Util::String::hexify(V[0x8])));
    ui->V9LineEdit->setText(QString::fromStdString(Util::String::hexify(V[0x9])));
    ui->VALineEdit->setText(QString::fromStdString(Util::String::hexify(V[0xA])));
    ui->VBLineEdit->setText(QString::fromStdString(Util::String::hexify(V[0xB])));
    ui->VCLineEdit->setText(QString::fromStdString(Util::String::hexify(V[0xC])));
    ui->VDLineEdit->setText(QString::fromStdString(Util::String::hexify(V[0xD])));
    ui->VELineEdit->setText(QString::fromStdString(Util::String::hexify(V[0xE])));
    ui->VFLineEdit->setText(QString::fromStdString(Util::String::hexify(V[0xF])));
}

void MainWindow::opcodeChanged() {
    handleOpcodeFrequencyAverage();
    ui->OpcodeLineEdit->setText(QString::fromStdString(Util::String::hexify(emulator->opcode())));
}

void MainWindow::iRegisterChanged() {
    ui->ILineEdit->setText(QString::fromStdString(Util::String::hexify(emulator->IRegister())));
}

void MainWindow::stackChanged() {
    std::stringstream stackContent;
    std::stack<std::uint16_t> stack = emulator->stack();

    if (stack.size() != 0) {
        stackContent << stack.top();
        stack.pop();
    } else {
        stackContent << "empty";
    }

    while (stack.size() > 0) {
        stackContent << "<--" << stack.top();
        stack.pop();
    }

    ui->StackLineEdit->setText(QString::fromStdString(stackContent.str()));
}

void MainWindow::pcChanged() {
    ui->PCLineEdit->setText(QString::fromStdString(Util::String::hexify(emulator->PC())));
}

void MainWindow::keyChanged() {
    std::vector<bool> keysAsVector = emulator->key();
    ui->key0LineEdit->setText(Util::String::toQString(keysAsVector[0x0]));
    ui->key1LineEdit->setText(Util::String::toQString(keysAsVector[0x1]));
    ui->key2LineEdit->setText(Util::String::toQString(keysAsVector[0x2]));
    ui->key3LineEdit->setText(Util::String::toQString(keysAsVector[0x3]));
    ui->key4LineEdit->setText(Util::String::toQString(keysAsVector[0x4]));
    ui->key5LineEdit->setText(Util::String::toQString(keysAsVector[0x5]));
    ui->key6LineEdit->setText(Util::String::toQString(keysAsVector[0x6]));
    ui->key7LineEdit->setText(Util::String::toQString(keysAsVector[0x7]));
    ui->key8LineEdit->setText(Util::String::toQString(keysAsVector[0x8]));
    ui->key9LineEdit->setText(Util::String::toQString(keysAsVector[0x9]));
    ui->keyALineEdit->setText(Util::String::toQString(keysAsVector[0xA]));
    ui->keyBLineEdit->setText(Util::String::toQString(keysAsVector[0xB]));
    ui->keyCLineEdit->setText(Util::String::toQString(keysAsVector[0xC]));
    ui->keyDLineEdit->setText(Util::String::toQString(keysAsVector[0xD]));
    ui->keyELineEdit->setText(Util::String::toQString(keysAsVector[0xE]));
    ui->keyFLineEdit->setText(Util::String::toQString(keysAsVector[0xF]));
}

void MainWindow::latestErrorChanged() {
    ui->logPlainTextEdit->appendPlainText(QString::fromStdString(emulator->latestError()));
}

void MainWindow::delayTimerChanged() {
    ui->delayTimerProgressBar->setValue(emulator->delayTimer());
}

void MainWindow::soundTimerChanged() {
    ui->soundTimerProgressBar->setValue(emulator->soundTimer());
}

void MainWindow::beep() {
    QSound::play("bell.wav");
}

void MainWindow::draw() {
    handleDrawFrequencyAverage();
    ui->gameRenderArea->display(emulator->display());
}

void MainWindow::statusChanged() {
    switch (emulator->status()) {
    case Emu::Running:
        ui->startStopButton->setEnabled(true);
        ui->pauseButton->setEnabled(true);
        ui->nextInstrButton->setEnabled(false);
        ui->startStopButton->setText("Stop");
        break;
    case Emu::Stopped:
        ui->startStopButton->setEnabled(true);
        ui->pauseButton->setEnabled(false);
        ui->nextInstrButton->setEnabled(true);
        ui->startStopButton->setText("Start");
        break;
    case Emu::Paused:
        ui->pauseButton->setEnabled(false);
        ui->nextInstrButton->setEnabled(true);
        ui->startStopButton->setText("Start");
        break;
    case Emu::FileNotLoaded:
        ui->startStopButton->setEnabled(false);
        ui->pauseButton->setEnabled(false);
        ui->nextInstrButton->setEnabled(false);
        break;
    }
}

void MainWindow::toggleDebugMenu() {
    if (ui->debugLayout->parentWidget()->isVisible())
        ui->debugLayout->parentWidget()->hide();
    else
        ui->debugLayout->parentWidget()->show();
}

void MainWindow::startStop() {
    if (emulator->status() == Emu::Running)
        emulator->stop();
    else
        emulator->start();
}

void MainWindow::pause() {
    emulator->pause();
}

void MainWindow::nextInstruction() {
    emulator->nextInstruction();
}

void MainWindow::openFile() {
    QString readFilename = QFileDialog::getOpenFileName(this, tr("Open file"), ".",
                                                        tr("chip8 files (*.ch8 *.c8 *.rom)"));

    if (readFilename != nullptr && readFilename != "") {
        filename = readFilename;
        Emu::EmulatorFileLoader fileLoader(filename.toStdString());
        emulator->loadFile(fileLoader.getLoadedFile());
    }
}

void MainWindow::openSettingsDialog() {
    SettingsDialog dialog = SettingsDialog(emulator->settings(), this);

    if (dialog.exec() == QDialog::Accepted) {
        Emu::EmulatorSettings newSettings = dialog.getNewEmulatorSettings();
        emulator->emulatorSettings(newSettings);
        ui->gameRenderArea->offColor(Util::Color::toQColor(newSettings.offColor));
        ui->gameRenderArea->onColor(Util::Color::toQColor(newSettings.onColor));
        settingsFileManager->write(newSettings);
    }
}

void MainWindow::handleDrawFrequencyAverage() {
    int frequency = emulator->settings().frequency;

    if (updateDrawFrequencyAverageCounter == drawFrequencyAverageFactor * static_cast<uint>(frequency)) {
        qint64 elapsedTime = drawTimerForAverage.nsecsElapsed();
        drawTimerForAverage.restart();

        elapsedTime /= updateDrawFrequencyAverageCounter;

        std::stringstream ss;
        if (elapsedTime != 0)
            ss << 1.0 / (static_cast<double>(elapsedTime) * NANO) << " Hz";
        else
            ss << "NaN";

        ui->DrawFreqAvgLineEdit->setText(QString::fromStdString(ss.str()));
        updateDrawFrequencyAverageCounter = 0;
    } else {
        ++updateDrawFrequencyAverageCounter;
    }
}

void MainWindow::handleOpcodeFrequencyAverage() {
    int frequency = emulator->settings().frequency;

    if (updateOpcodeFrequencyAverageCounter == opcodeFrequencyAverageFactor * static_cast<uint>(frequency)) {
        qint64 elapsedTime = opcodeTimerForAverage.nsecsElapsed();
        opcodeTimerForAverage.restart();

        elapsedTime /= updateOpcodeFrequencyAverageCounter;

        std::stringstream ss;
        if (elapsedTime != 0)
            ss << 1.0 / (static_cast<double>(elapsedTime) * NANO) << " Hz";
        else
            ss << "NaN";

        ui->OpcodeFreqAvgLineEdit->setText(QString::fromStdString(ss.str()));
        updateOpcodeFrequencyAverageCounter = 0;
    } else {
        ++updateOpcodeFrequencyAverageCounter;
    }
}
