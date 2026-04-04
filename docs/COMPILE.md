# COMPILE

You first need to have **vcpkg** pre-installed using either ``.
In case you have installed it, be sure to set `$VCPKG_ROOT` to the location of the program. You also need to install **cmake** to run build commands.

## Required libraries

 - SDL3
 - SDL3_image
 - SDL3_ttf

## Using terminal

It's independanly from the distribution family or operating system, the process of compiling is similar using terminal.

In case you want default usage of cmake using: `mkdir build; cd build; cmake ..`, `CMakeLists.txt` wasn't designed to deal with cases like this,
so run this command on your considiration

### in Windows

You may need to install Visual Studio, specifically `MSVC Build tools for x64/x86 (Latest?)` and `Windows [windows version] SDK`.
`Just-In-Time Debugging` and `MSVC AddressSanitizer` would be handy FOR DEBUGGING ONLY.
The reason of installation is the dependancies on build tooling for `vcpkg`.
If you would manage to avoid tooling dependancy of `vcpkg` from Visual Studio, it would be fine as well.

### in Linux family

You should first install all tooling it needs, such as:
	`git`, `cmake`, `make (or your prefered generator but you would need to add "-G"(your generator defined in CMake)" to override properties of presets")`.
In addition you would need to install all dependencies for SDL3 itself following [this page](https://wiki.libsdl.org/SDL3/README-linux).
The reason of installing whole bundle just to work with SDL3 is because it's features could not work as it should after compiling SDL3, so INSTALL all these dependancies BEFORE COMPILING WITH CMAKE COMMAND.
If you would accidentally do this, you should then delete `~/.config/vcpkg/` directory where all cached binaries are and retry compiling again.

### after all that...

Run `cmake` with presets. To look which preset you want, type `cmake --list-presets`, and run `cmake --preset=<YOUR_CHOICE>`.
Same thing would be to building, using `cmake --build --list-presets` to look up which preset you may want, and run
`cmake --build --preset=<YOUR_CHOICE>`.

Compiled program should be at that order of directories `/out/(build preset name)-build/Wanichusilk(|.exe|.elf|.*)`

If you would know better or easier tooling than that, lmk.

## in Windows using dedicated IDE

In case of compiling in dedicated Visual Studio Enviroument (in IDE), you need to be sure that vcpkg works and installs correctly.
After that you can now generate cmake configuration, and then you can normally compile.

## in Linux family in addition

The compilation usually happen within terminal, so follow these steps from above.

If in stage of compiling packages in vcpkg you getting an error when it tries to compile `libxcrypt`, be sure to have a package `perl`.

