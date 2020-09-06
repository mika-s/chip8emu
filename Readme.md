# chip8emu

This is yet another Chip8 emulator. It's made in C++ and uses Qt as frontend framework.

It generally works as it should. The games I've tried works as intended. Improvements
could be made with respect to the timing. At the moment the emulator timer is
implemented as a QTimer that runs at 60 Hz (can be configured), and a new instruction
is fetched 9 times (can also be configured) per QTimer tick. The timer runs on the
main thread, so the CPU load is high. Anti-flickering has not been implemented.

The emulator has only been tested on Linux. It uses C++17 (GCC), Qt 5.15.0 and CMake minimum 3.5.

![Emulator running](assets/src-running.png?raw=true)

![Emulator settings](assets/src-settings.png?raw=true)

## To build

In the root directory:

```
cmake '-GCodeBlocks - Ninja' -B build
cmake --build build/
```

The emulator binary is now in the build/src directory.

## Improvements that could be made

- QTimer in separate thread.
- More unittests.
- Are keys platform independent?
- Implement Superchip opcodes.
- Anti-flickering.
