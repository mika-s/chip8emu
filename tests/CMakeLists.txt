cmake_minimum_required(VERSION 3.5)

project(emulatortest LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5Test REQUIRED)
find_package(Qt5 COMPONENTS Widgets Multimedia REQUIRED)

add_library(emulatorlib
    ${chip8emu_SOURCE_DIR}/src/emulator.cpp                    ${chip8emu_SOURCE_DIR}/src/emulator.h
    ${chip8emu_SOURCE_DIR}/src/emulatorstatus.h
    ${chip8emu_SOURCE_DIR}/src/emulatorfileloader.cpp          ${chip8emu_SOURCE_DIR}/src/emulatorfileloader.h
    ${chip8emu_SOURCE_DIR}/src/emulatortimer.cpp               ${chip8emu_SOURCE_DIR}/src/emulatortimer.h
    ${chip8emu_SOURCE_DIR}/src/emulatorsettings.cpp            ${chip8emu_SOURCE_DIR}/src/emulatorsettings.h
    ${chip8emu_SOURCE_DIR}/src/emulatorsettingsfilemanager.cpp ${chip8emu_SOURCE_DIR}/src/emulatorsettingsfilemanager.h
    ${chip8emu_SOURCE_DIR}/src/idebuglistener.h
    ${chip8emu_SOURCE_DIR}/src/ierrorlistener.h
    ${chip8emu_SOURCE_DIR}/src/iiolistener.h
    ${chip8emu_SOURCE_DIR}/src/itimerlistener.h
    ${chip8emu_SOURCE_DIR}/src/itimer.h
    ${chip8emu_SOURCE_DIR}/src/invalidkeyexception.cpp         ${chip8emu_SOURCE_DIR}/src/invalidkeyexception.h
    ${chip8emu_SOURCE_DIR}/src/filetoolargeexception.cpp       ${chip8emu_SOURCE_DIR}/src/filetoolargeexception.h
    ${chip8emu_SOURCE_DIR}/src/pcoutofboundsexception.cpp      ${chip8emu_SOURCE_DIR}/src/pcoutofboundsexception.h
    ${chip8emu_SOURCE_DIR}/src/unrecognizedopcodeexception.cpp ${chip8emu_SOURCE_DIR}/src/unrecognizedopcodeexception.h
    ${chip8emu_SOURCE_DIR}/src/util.cpp                        ${chip8emu_SOURCE_DIR}/src/util.h
)

file(GLOB SOURCES_CPP "*.cpp")
file(GLOB SOURCES_H "*.h")

add_executable(${PROJECT_NAME} ${SOURCES_CPP} ${SOURCES_H})

include_directories (${chip8emu_SOURCE_DIR}/src)

target_link_libraries(emulatorlib PRIVATE Qt5::Widgets Qt5::Multimedia)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Test emulatorlib)
