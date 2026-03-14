# COMPILE

You first need to have **vcpkg** pre-installed, in case you have installed it, be sure to set `$VCPKG_ROOT` to the location of the program. You also need to install **cmake** to run build commands.

## using terminal
It's independanly from the distribution family or operating system, the process of compiling is similar using terminal.

In case you want default usage of cmake using: `mkdir build; cd build; cmake ..`, `CMakeLists.txt` wasn't designed to deal with cases like this,
so run this command on your considiration

Run `cmake` with presets. To look which preset you want, type `cmake --list-presets`, and run `cmake --preset=<YOUR_CHOICE>`.
Same thing would be to building, using `cmake --build --list-presets` to look up which preset you may want, and run
`cmake --build --preset=<YOUR_CHOICE>`.

Compiled program should be at that order of directories `/out/build/(build preset name)/Wanichusilk(|.exe|.elf|.*)`

## in Windows

In case of compiling in Visual Studio Enviroument, you need to be sure that vcpkg works and installs correctly.
After that you can now generate cmake configuration, and then you can normally compile.

## in Linux family

The compilation usually happen within terminal, so follow these steps from above.

If in stage of compiling packages in vcpkg you getting an error when it tries to compile `libxcrypt`, be sure to have a package `perl`.

