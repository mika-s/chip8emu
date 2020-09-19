cmake_minimum_required(VERSION 3.5)

project(emulator LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets Multimedia REQUIRED)

file(GLOB SOURCES_CPP "*.cpp")
file(GLOB SOURCES_H "*.h")

add_executable(${PROJECT_NAME} ${SOURCES_CPP} ${SOURCES_H})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets Qt5::Multimedia)

configure_file(settings.json settings.json COPYONLY)
configure_file(bell.wav bell.wav COPYONLY)
