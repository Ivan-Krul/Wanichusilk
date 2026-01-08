# COMPILE

You first need to have **vcpkg** pre-installed, in case you have installed it, be sure to set `$VCPKG_ROOT` to the location of the program. You also need to install **cmake** to run build commands.

## in Windows

In case of compiling in Visual Studio, you need first run vcpkg with command tool and not with-in build stage on the IDE.
After that you can now generate cmake configuration, and then you can normally compile.

## in Linux family

Independanly from the distribution family, the process of compiling is similar.
Run `cmake` with presets. To look which preset you want, type `cmake --list-presets`, and run `cmake --preset=<YOUR_CHOICE>`.

If in stage of compiling packages in vcpkg you getting an error when it tries to compile `libxcrypt`, be sure to have a package `perl`.

Same thing would be to building, using `cmake --build --list-presets` to look up which preset you may want, and run
`cmake --build --preset=<YOUR_CHOICE>`.

