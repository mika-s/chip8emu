cmake_minimum_required(VERSION 3.5)

project(emulator LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets Multimedia REQUIRED)

add_executable(emulator
  main.cpp
  mainwindow.cpp mainwindow.h mainwindow.ui
  emulator.cpp emulator.h emulatorstatus.h
  emulatorfileloader.cpp emulatorfileloader.h
  emulatortimer.cpp emulatortimer.h
  emulatorsettings.cpp emulatorsettings.h
  emulatorsettingsfilemanager.cpp emulatorsettingsfilemanager.h
  gamerenderarea.cpp gamerenderarea.h
  idebuglistener.h ierrorlistener.h iiolistener.h itimerlistener.h itimer.h
  invalidkeyexception.cpp invalidkeyexception.h
  filetoolargeexception.cpp filetoolargeexception.h
  pcoutofboundsexception.cpp pcoutofboundsexception.h
  unrecognizedopcodeexception.cpp unrecognizedopcodeexception.h
  settingsdialog.cpp settingsdialog.h settingsdialog.ui
  util.cpp util.h
)

target_link_libraries(emulator PRIVATE Qt5::Widgets Qt5::Multimedia)

configure_file(settings.json settings.json COPYONLY)
configure_file(bell.wav bell.wav COPYONLY)
