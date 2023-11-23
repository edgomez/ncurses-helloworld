# NCurses Hello World in C++

## Description

Simple Hello World program using the ncurses library.

## How to build

This follows the typical CMake based build instruction workflow.

```bash
cmake -Bbuild/host -G "Ninja Multi-Config"
cmake --build build/host --config Debug
```

**NB**: The program has been compiled and tested on a typical GNU/Linux system with ncurses installed.

## Usage

The program is really just a Hello World program.

```bash
./build/host/Debug/ncurses-helloworld
```

Then, press <kbd>q</kbd> to quit.

## License

See [./LICENSE.md](./LICENSE.md)
