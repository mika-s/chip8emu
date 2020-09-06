cmake_minimum_required(VERSION 3.5)

project(chip8emu LANGUAGES CXX)

add_subdirectory (src)
add_subdirectory (tests)

add_test(NAME emulatortest COMMAND emulatortest)
enable_testing(true)
